#!/usr/bin/perl
# -*- coding: utf-8 -*-
#
# SboSvr/ 以下の全 .h/.cpp ファイルのコードスタイルを整理するスクリプト
#
use strict;
use warnings;
use utf8;
use Encode qw(decode encode);
use File::Find;
use File::Basename;

binmode(STDOUT, ':encoding(utf-8)');
binmode(STDERR, ':encoding(utf-8)');

# 制御構文キーワード（関数呼び出し空白除去の対象外）
my %ctrl_kw = map { $_ => 1 } qw(
    if else for while do switch return catch throw delete new case typedef struct
    enum namespace class template operator __declspec __attribute__ sizeof
    static_cast reinterpret_cast const_cast dynamic_cast
);

my $target_dir = 'C:/Users/masakazu/MyProject/SBOP2/SboSvr';

my @files;
find(sub {
    if (-f $_ && ($_ =~ /\.h$/ || $_ =~ /\.cpp$/)) {
        push @files, $File::Find::name;
    }
}, $target_dir);
@files = sort @files;
print "対象ファイル数: " . scalar(@files) . "\n";

my $changed = 0;
for my $filepath (@files) {
    my $result = eval { process_file($filepath) };
    if ($@) {
        warn "エラー: $filepath: $@\n";
        next;
    }
    if ($result) {
        $changed++;
        print "  変更: $filepath\n";
    } else {
        print "  スキップ: $filepath\n";
    }
}
print "\n完了: $changed/" . scalar(@files) . " ファイルを変更しました\n";

# =========================================================================
sub process_file {
    my ($filepath) = @_;

    # バイナリで読み込み
    open(my $raw, '<:raw', $filepath) or die "読み込みエラー: $!\n";
    my $raw_bytes;
    { local $/; $raw_bytes = <$raw>; }
    close($raw);

    # BOM除去
    my $has_bom = 0;
    if (length($raw_bytes) >= 3 && substr($raw_bytes, 0, 3) eq "\xEF\xBB\xBF") {
        $has_bom = 1;
        $raw_bytes = substr($raw_bytes, 3);
    }

    # CRLF→LF
    $raw_bytes =~ s/\r\n/\n/g;
    $raw_bytes =~ s/\r/\n/g;

    # UTF-8デコード（失敗したらcp932）
    my $content;
    eval { $content = decode('UTF-8', $raw_bytes, Encode::FB_CROAK); };
    if ($@) {
        eval { $content = decode('cp932', $raw_bytes, Encode::FB_CROAK); };
        if ($@) {
            $content = decode('latin-1', $raw_bytes);
        }
    }

    my @lines = split(/\n/, $content, -1);

    # === 項目1: ファイルヘッダをDoxygen形式に変換 ===
    @lines = convert_file_header(@lines);

    # === 項目2+5: 関数ヘッダの削除・セクションコメントの簡略化 ===
    @lines = process_separator_blocks(@lines);

    # === 項目6: 残った /* ... */ を // に変換 ===
    @lines = convert_block_comments_to_line(@lines);

    # === 項目3: 関数呼び出し時の空白を削除 ===
    @lines = remove_func_spaces(@lines);

    # === 項目4: 複数タブを1タブに ===
    @lines = collapse_multi_tabs(@lines);

    # === 末尾フッターの削除 ===
    @lines = remove_trailing_footer(@lines);

    # === 連続空行を最大1行に ===
    @lines = collapse_blank_lines(@lines);

    my $new_content = join("\n", @lines) . "\n";

    # 変更がなければスキップ
    return 0 if $new_content eq $content;

    # UTF-8（BOMなし・LF）で書き戻す
    open(my $out, '>:raw', $filepath) or die "書き込みエラー: $!\n";
    print $out encode('UTF-8', $new_content);
    close($out);
    return 1;
}

# =========================================================================
# 項目1: 旧式ファイルヘッダ → Doxygen形式
# 変換例:
#   /* Copyright(C)URARA-works 2007 */
#   /* ========================================================================= */
#   /* ファイル名	:SboSvr.h    */
#   /* 内容		:説明         */
#   /* 作成		:作者名        */
#   /* 作成開始日	:2007/05/01   */
#   /* ========================================================================= */
# ↓
#   /// @file SboSvr.h
#   /// @brief 説明
#   /// @author 作者名
#   /// @date 2007/05/01
#   /// @copyright Copyright(C)URARA-works 2007
# =========================================================================
sub convert_file_header {
    my @lines = @_;
    return @lines unless @lines >= 2;

    # 1行目: /* Copyright(C)... */
    return @lines unless $lines[0] =~ m{/\*\s*Copyright\(C\)(.*?)\s*\*/};
    my $copyright_suffix = $1;
    $copyright_suffix =~ s/^\s+|\s+$//g;
    my $copyright = "Copyright(C)$copyright_suffix";

    # 2行目: /* ===...=== */
    return @lines unless $lines[1] =~ m{/\*\s*=+\s*\*/};

    # 3行目以降: フィールド行を収集して終端区切りを探す
    my %fields;
    my $idx = 2;
    while ($idx < scalar(@lines)) {
        my $line = $lines[$idx];
        # 終端区切り: /* ===...=== */
        if ($line =~ m{/\*\s*=+\s*\*/}) {
            $idx++;
            last;
        }
        # フィールド行のパース
        # 形式: /* キー名\t\t:値\t\t\t... */
        # または: /* キー名    :値       */
        # \xEF\xBC\x9A は全角コロン
        if ($line =~ m{/\*\s*(.+?)[\t ]+:(.+?)\s*\*/}) {
            my $key = $1;
            my $val = $2;
            $key =~ s/\s+$//g;
            # 値の末尾パディング（タブ・空白）を除去
            $val =~ s/[\t ]+$//g;
            $fields{$key} = $val;
        }
        $idx++;
    }

    my $filename = $fields{'ファイル名'} // '';
    my $brief    = $fields{'内容'}       // '';
    my $author   = $fields{'作成'}       // '';
    my $date     = $fields{'作成開始日'} // '';

    # 末尾の余分な空白・タブを再除去
    $filename =~ s/[\t ]+$//g;
    $brief    =~ s/[\t ]+$//g;
    $author   =~ s/[\t ]+$//g;
    $date     =~ s/[\t ]+$//g;

    my @doxy;
    push @doxy, "/// \@file $filename"     if $filename ne '';
    push @doxy, "/// \@brief $brief"       if $brief ne '';
    push @doxy, "/// \@author $author"     if $author ne '';
    push @doxy, "/// \@date $date"         if $date ne '';
    push @doxy, "/// \@copyright $copyright";

    return (@doxy, '', @lines[$idx .. $#lines]);
}

# =========================================================================
# 項目2+5: 区切りブロック /* ===...=== */ の処理
#   - 「関数名」を含む → 関数ヘッダとして削除
#   - 3行ブロック（セクション）→ // 見出し に簡略化
# =========================================================================
sub process_separator_blocks {
    my @lines = @_;
    my @result;
    my $i = 0;
    my $n = scalar @lines;

    while ($i < $n) {
        my $line = $lines[$i];

        # /* ===...=== */ で始まるブロック開始を検出
        if ($line =~ m{^\s*/\*\s*=+\s*\*/\s*$}) {
            # 終端区切りまで収集
            my @block = ($line);
            my $j = $i + 1;
            my $found_end = 0;
            while ($j < $n) {
                push @block, $lines[$j];
                if ($lines[$j] =~ m{^\s*/\*\s*=+\s*\*/\s*$}) {
                    $j++;
                    $found_end = 1;
                    last;
                }
                $j++;
            }

            if (!$found_end) {
                push @result, $line;
                $i++;
                next;
            }

            my $block_text = join("\n", @block);

            if ($block_text =~ /関数名/) {
                # 関数ヘッダ → 削除
                $i = $j;
                # 直前の余分な空行を除去
                while (@result && $result[-1] =~ /^\s*$/) {
                    pop @result;
                }
                push @result, '';
                # ブロック後の空行をスキップ
                while ($i < $n && $lines[$i] =~ /^\s*$/) {
                    $i++;
                }
                next;
            } else {
                # セクションヘッダ → 簡略化
                # block[0]=開始区切り, block[1..n-2]=内容行, block[-1]=終端区切り
                my @content_lines;
                for my $bi (1 .. $#block - 1) {
                    my $bline = $block[$bi];
                    # /* 見出し */ の形式から見出しを取り出す
                    if ($bline =~ m{/\*+\s*(.*?)\s*\*/}) {
                        my $h = $1;
                        $h =~ s/^\s+|\s+$//g;
                        push @content_lines, $h if $h ne '';
                    }
                }
                if (@content_lines) {
                    for my $h (@content_lines) {
                        push @result, "// $h";
                    }
                    $i = $j;
                    next;
                }
                # 変換できない場合はそのまま
                push @result, @block;
                $i = $j;
                next;
            }
        }

        push @result, $line;
        $i++;
    }
    return @result;
}

# =========================================================================
# 項目6: 残った /* ... */ を // に変換
# =========================================================================
sub convert_block_comments_to_line {
    my @lines = @_;
    my @result;
    my $in_block = 0;
    my @block_buf;
    my $block_indent = '';

    for my $line (@lines) {
        if ($in_block) {
            if ($line =~ m{^(.*?)\*/(.*)$}) {
                my ($before, $after) = ($1, $2);
                $before =~ s/^\s*\*?\s*//;
                $before =~ s/\s+$//;
                push @block_buf, $before if $before ne '';
                $in_block = 0;
                for my $bc (@block_buf) {
                    push @result, "${block_indent}// $bc";
                }
                @block_buf = ();
                $after =~ s/^\s+|\s+$//g;
                push @result, "${block_indent}$after" if $after ne '';
            } else {
                my $content = $line;
                $content =~ s/^\s*\*?\s*//;
                $content =~ s/\s+$//;
                push @block_buf, $content if $content ne '';
            }
            next;
        }

        # 行が /* で始まる（インデントあり可）
        if ($line =~ m{^(\s*)/\*(.*)$}) {
            my ($indent, $rest) = ($1, $2);
            if ($rest =~ m{^(.*?)\*/(.*)$}) {
                # 1行完結
                my ($body, $after) = ($1, $2);
                $body =~ s/^\s+|\s+$//g;
                $after =~ s/^\s+|\s+$//g;
                push @result, "${indent}// $body" if $body ne '';
                push @result, "${indent}$after"   if $after ne '';
            } else {
                # 複数行ブロック開始
                $in_block = 1;
                $block_indent = $indent;
                $rest =~ s/^\s+|\s+$//g;
                push @block_buf, $rest if $rest ne '';
            }
            next;
        }

        # 行末インラインコメント: code  /* comment */
        if ($line =~ m{^(.*?)\s*/\*(.*?)\*/\s*$}) {
            my ($code, $comment) = ($1, $2);
            $code    =~ s/\s+$//;
            $comment =~ s/^\s+|\s+$//g;
            $line = $comment ne '' ? "$code\t// $comment" : $code;
        }

        push @result, $line;
    }

    return @result;
}

# =========================================================================
# 項目3: 関数呼び出し時の空白を削除
#   identifier (  →  identifier(
#   ただし制御構文キーワードは除く
# =========================================================================
sub remove_func_spaces {
    my @lines = @_;
    my @result;
    for my $line (@lines) {
        # #define 行は変換しない（マクロ名と ( の間の空白は意味を持つ）
        if ($line =~ m{^\s*#\s*define\b}) {
            push @result, $line;
            next;
        }

        # 行を コード部 と // コメント部 に分離
        my ($code, $comment) = split_at_line_comment($line);

        # コード部: 識別子 + 空白 + ( を変換（制御構文以外）
        $code =~ s/\b(\w+)([ \t]+)\(/
            $ctrl_kw{$1} ? "$1$2(" : "$1("
        /ge;

        push @result, $code . $comment;
    }
    return @result;
}

# =========================================================================
# 項目4: 複数タブを1タブに変換
# =========================================================================
sub collapse_multi_tabs {
    my @lines = @_;
    my @result;
    for my $line (@lines) {
        my ($code, $comment) = split_at_line_comment($line);
        # 行頭インデント（タブ）を保持しつつ、それ以降の連続タブを1タブに変換
        if ($code =~ m{^(\t*)(.*)$}s) {
            my ($indent, $rest) = ($1, $2);
            $rest =~ s/\t{2,}/\t/g;
            $code = $indent . $rest;
        }
        push @result, $code . $comment;
    }
    return @result;
}

# =========================================================================
# 末尾フッター削除: /* Copyright(C)... */ または // Copyright(C)... を削除
# =========================================================================
sub remove_trailing_footer {
    my @lines = @_;
    while (@lines && $lines[-1] =~ /^\s*$/) {
        pop @lines;
    }
    if (@lines && $lines[-1] =~ m{(?:/\*|//)\s*Copyright\(C\).*(?:\*/)?$}) {
        pop @lines;
    }
    while (@lines && $lines[-1] =~ /^\s*$/) {
        pop @lines;
    }
    return @lines;
}

# =========================================================================
# 連続する空行を最大1行に
# =========================================================================
sub collapse_blank_lines {
    my @lines = @_;
    my @result;
    my $blank = 0;
    for my $line (@lines) {
        if ($line =~ /^\s*$/) {
            $blank++;
            push @result, '' if $blank <= 1;
        } else {
            $blank = 0;
            push @result, $line;
        }
    }
    return @result;
}

# =========================================================================
# ユーティリティ: 行をコード部と // コメント部に分離
# 文字列リテラル内の // は無視する
# =========================================================================
sub split_at_line_comment {
    my ($line) = @_;
    my $pos = 0;
    my $len = length($line);
    my $in_str = '';

    while ($pos < $len) {
        my $c = substr($line, $pos, 1);
        if ($in_str) {
            if ($c eq '\\') {
                $pos += 2;
                next;
            }
            $in_str = '' if $c eq $in_str;
            $pos++;
            next;
        }
        if ($c eq '"' || $c eq "'") {
            $in_str = $c;
            $pos++;
            next;
        }
        if ($c eq '/' && $pos + 1 < $len && substr($line, $pos + 1, 1) eq '/') {
            return (substr($line, 0, $pos), substr($line, $pos));
        }
        $pos++;
    }
    return ($line, '');
}
