# SboSvr のワイヤプロトコル検証
#
# 通信実装(WSAAsyncSelect版 / select版)を入れ替えても挙動が変わらないことを
# 確認するための回帰スクリプト。サーバーを起動した状態で実行する。
#
#   $env:SBO_SOCK_IMPL = ""        # 既存実装
#   $env:SBO_SOCK_IMPL = "select"  # select実装
#   SboSvr.exe --headless
#   pwsh tools/test-sbosvr-protocol.ps1 -Port 2017
#
# サーバー側に Update\SBOHashList.txt が必要(ファイルリスト応答の検証で使う)。
#
# 検証内容:
#   1. プリチェック + VERSION チェック往復
#   2. zlib 圧縮した大きい要求 (サーバーの展開経路)
#   3. 大きい応答の受信 (サーバーの圧縮経路)
#   4. 複数同時接続
#   5. 切断の検知
param([int]$Port = 2017)

$ErrorActionPreference = "Stop"
$KEY = 0x56BB3E5E
$VERSIONVAL = 0x00004300
$ok = 0; $ng = 0

function Ok($m)  { Write-Output "  OK   $m"; $script:ok++ }
function Ng($m)  { Write-Output "  NG   $m"; $script:ng++ }

# --- CRC-32 (RFC1952) ---
$tbl = New-Object uint32[] 256
for ($i = 0; $i -lt 256; $i++) {
    $c = [uint32]$i
    for ($k = 0; $k -lt 8; $k++) {
        if ($c -band 1) { $c = [uint32](([uint32]3988292384) -bxor ($c -shr 1)) } else { $c = [uint32]($c -shr 1) }
    }
    $tbl[$i] = $c
}
function Get-Crc32([byte[]]$buf) {
    $c = [uint32]::MaxValue
    foreach ($b in $buf) { $c = [uint32]($tbl[[int](($c -bxor $b) -band 0xFF)] -bxor ($c -shr 8)) }
    return [uint32]($c -bxor [uint32]::MaxValue)
}
function Compress-Zlib([byte[]]$raw) {
    $ms = New-Object System.IO.MemoryStream
    $zs = New-Object System.IO.Compression.ZLibStream($ms, [System.IO.Compression.CompressionMode]::Compress, $true)
    $zs.Write($raw, 0, $raw.Length); $zs.Dispose()
    return $ms.ToArray()
}
function Expand-Zlib([byte[]]$comp, [int]$outSize) {
    $ms = New-Object System.IO.MemoryStream(,$comp)
    $zs = New-Object System.IO.Compression.ZLibStream($ms, [System.IO.Compression.CompressionMode]::Decompress)
    $out = New-Object byte[] $outSize
    $got = 0
    while ($got -lt $outSize) {
        $n = $zs.Read($out, $got, $outSize - $got)
        if ($n -le 0) { break }
        $got += $n
    }
    $zs.Dispose()
    return $out[0..($got-1)]
}
function Read-Exact($s, [int]$len) {
    $b = New-Object byte[] $len; $got = 0
    while ($got -lt $len) {
        $n = $s.Read($b, $got, $len - $got)
        if ($n -le 0) { return $null }
        $got += $n
    }
    return $b
}
function New-Frame([byte[]]$body) {
    $f = New-Object byte[] (8 + $body.Length)
    [Array]::Copy([BitConverter]::GetBytes([uint32]$body.Length), 0, $f, 0, 4)
    [Array]::Copy([BitConverter]::GetBytes((Get-Crc32 $body)), 0, $f, 4, 4)
    if ($body.Length) { [Array]::Copy($body, 0, $f, 8, $body.Length) }
    return $f
}
# 本体を zlib 圧縮して 0xFF + 元サイズ を前置する(既存実装と同じ形)
function New-CompressedBody([byte[]]$raw) {
    $c = Compress-Zlib $raw
    $b = New-Object byte[] (5 + $c.Length)
    $b[0] = 0xFF
    [Array]::Copy([BitConverter]::GetBytes([uint32]$raw.Length), 0, $b, 1, 4)
    [Array]::Copy($c, 0, $b, 5, $c.Length)
    return $b
}

function Connect-Sbo() {
    $cli = New-Object System.Net.Sockets.TcpClient
    $cli.Connect("127.0.0.1", $Port)
    $ns = $cli.GetStream(); $ns.ReadTimeout = 8000
    $null = Read-Exact $ns 8
    $ch = [BitConverter]::ToUInt32((Read-Exact $ns 4), 0)
    $ans = [uint32]((([uint64]($ch -band $KEY) * [uint64]$KEY)) % 4294967296)
    $ns.Write((New-Frame ([BitConverter]::GetBytes($ans))), 0, 12)
    $ns.Flush()
    return @{ Client = $cli; Stream = $ns }
}
# 応答を1つ受け取り、圧縮されていれば展開して返す
function Recv-Packet($ns) {
    $h = Read-Exact $ns 8
    if ($h -eq $null) { return $null }
    $size = [BitConverter]::ToUInt32($h, 0)
    if ($size -eq 0) { return @() }
    $body = Read-Exact $ns ([int]$size)
    if ($body -eq $null) { return $null }
    if ($body[0] -eq 0xFF) {
        $orig = [BitConverter]::ToUInt32($body, 1)
        $script:LastWasCompressed = $true
        return (Expand-Zlib $body[5..($body.Length-1)] ([int]$orig))
    }
    $script:LastWasCompressed = $false
    return $body
}

Write-Output "=== Port $Port ==="

# --- 1. 基本の往復 ---
$s = Connect-Sbo
$body = New-Object byte[] 6
$body[0] = 1; $body[1] = 1
[Array]::Copy([BitConverter]::GetBytes([uint32]$VERSIONVAL), 0, $body, 2, 4)
$f = New-Frame $body
$s.Stream.Write($f, 0, $f.Length); $s.Stream.Flush()
$r = Recv-Packet $s.Stream
if ($r -and $r[0] -eq 1 -and $r[1] -eq 2 -and $r[2] -eq 1) { Ok "VERSIONチェック往復 (result=1)" } else { Ng "VERSIONチェック往復" }

# --- 2. zlib 圧縮した大きい要求 → サーバーの展開経路 ---
$raw = New-Object byte[] 400
$raw[0] = 1; $raw[1] = 1
[Array]::Copy([BitConverter]::GetBytes([uint32]$VERSIONVAL), 0, $raw, 2, 4)
for ($i = 6; $i -lt 400; $i++) { $raw[$i] = [byte](($i * 7) % 251) }
$cb = New-CompressedBody $raw
$f = New-Frame $cb
$s.Stream.Write($f, 0, $f.Length); $s.Stream.Flush()
$r = Recv-Packet $s.Stream
if ($r -and $r[0] -eq 1 -and $r[1] -eq 2) { Ok "zlib圧縮した400バイト要求をサーバーが展開 (圧縮後 $($cb.Length) バイト)" } else { Ng "zlib圧縮要求の展開" }

# --- 3. 大きい応答 → サーバーの圧縮経路 ---
$body = New-Object byte[] 2
$body[0] = 1; $body[1] = 5     # VERSION_REQ_FILELIST
$f = New-Frame $body
$s.Stream.Write($f, 0, $f.Length); $s.Stream.Flush()
$r = Recv-Packet $s.Stream
if ($r -and $r[0] -eq 1 -and $r[1] -eq 6) {
    if ($script:LastWasCompressed) { Ok "ファイルリスト応答を受信し展開 ($($r.Length) バイト、圧縮されていた)" }
    else { Ok "ファイルリスト応答を受信 ($($r.Length) バイト、非圧縮=128未満)" }
} else { Ng "ファイルリスト応答" }
$s.Client.Close()

# --- 4. 複数同時接続 ---
$conns = @()
for ($i = 0; $i -lt 5; $i++) { $conns += (Connect-Sbo) }
$allOk = $true
foreach ($c in $conns) {
    $body = New-Object byte[] 6
    $body[0] = 1; $body[1] = 1
    [Array]::Copy([BitConverter]::GetBytes([uint32]$VERSIONVAL), 0, $body, 2, 4)
    $f = New-Frame $body
    $c.Stream.Write($f, 0, $f.Length); $c.Stream.Flush()
}
foreach ($c in $conns) {
    $r = Recv-Packet $c.Stream
    if (-not ($r -and $r[0] -eq 1 -and $r[1] -eq 2)) { $allOk = $false }
}
if ($allOk) { Ok "5本同時接続でそれぞれ応答" } else { Ng "5本同時接続" }
foreach ($c in $conns) { $c.Client.Close() }

# --- 5. 切断後も新規接続を受け付ける ---
Start-Sleep -Milliseconds 500
$s2 = Connect-Sbo
$body = New-Object byte[] 6
$body[0] = 1; $body[1] = 1
[Array]::Copy([BitConverter]::GetBytes([uint32]$VERSIONVAL), 0, $body, 2, 4)
$f = New-Frame $body
$s2.Stream.Write($f, 0, $f.Length); $s2.Stream.Flush()
$r = Recv-Packet $s2.Stream
if ($r -and $r[0] -eq 1 -and $r[1] -eq 2) { Ok "切断後の再接続" } else { Ng "切断後の再接続" }
$s2.Client.Close()

Write-Output "=== 成功 $ok / 失敗 $ng ==="
if ($ng -gt 0) { exit 1 }
