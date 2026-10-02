# フォント

ゲーム画面の文字は、絵に合わせてピクセルフォントを 2 倍（1 ドット = 2x2 ピクセル）で描いています。
描画は `SboCli/src/Platform/SdlFont.cpp` にまとまっていて、要求された大きさでフォントを選びます。

| 要求サイズ | 使うフォント | 画面での大きさ | 主な用途 |
|---|---|---|---|
| 14px 以下 | 美咲ゴシック第2（8 ドット） | 16px | キャラ名、発言、ログ、細かい表示 |
| 15〜23px | PixelMplus10（10 ドット） | 20px | メニュー、ウィンドウの本文 |
| 24px 以上 | PixelMplus12（12 ドット） | 24px | マップ名などの大きい文字 |

Noto Sans CJK は ImGui（デバッグ用の画面）だけで使っています。

## 収録フォントとライセンス

### 美咲ゴシック第2（misaki_gothic_2nd.ttf）

- 作者: 門真 なむ（Num Kadoma）
- 配布元: https://littlelimit.net/misaki.htm
- ライセンス（配布元の記載より）: これらのフォントはフリー（自由な）ソフトウエアです。
  あらゆる改変の有無に関わらず、また商業的な利用であっても、自由にご利用、複製、再配布することが
  できますが、全て無保証とさせていただきます。

### PixelMplus10 / PixelMplus12（PixelMplus*.ttf）

- 作者: Itou Hiroki（M+ FONTS をもとに作成）
- 配布元: https://github.com/itouhiro/PixelMplus
- ライセンス: M+ FONT LICENSE

```
M+ FONTS                                Copyright (C) 2002-2013 M+ FONTS PROJECT

これらのフォントはフリー（自由な）ソフトウエアです。
あらゆる改変の有無に関わらず、また商業的な利用であっても、自由にご利用、
複製、再配布することができますが、全て無保証とさせていただきます。
```

### Noto Sans CJK JP（NotoSansCJKjp-*.otf）

- SIL Open Font License 1.1
