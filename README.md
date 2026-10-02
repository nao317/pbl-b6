# pbl-b6

後期PBLのソースコードとドキュメントを共有するリポジトリです。

## Windows + Zed + Arduino CLIでArduinoを開発する

Windows上のZedでArduinoのスケッチを書き、Arduino CLIでコンパイルと書き込みを行う手順です。対象はArduino Uno、ターミナルはPowerShellを想定しています。

初回は「Arduino CLIをインストール」から「Arduinoへ書き込む」までを順に進めてください。環境構築が済んでいる場合は、「開発するときの基本サイクル」から読めば足ります。

## ZedとArduino CLIの役割をつかむ

作業の流れは、Zedで編集し、ZedのターミナルからArduino CLIを実行し、USB経由でArduinoへ書き込むというものです。

```text
Zed
├── Arduinoコードを編集 (.ino)
│
└── Zed Terminal
      │
      └── arduino-cli
            ├── ボードの確認
            ├── Coreのインストール
            ├── コンパイル
            └── Arduinoへ書き込み
                    │
                    └── USB / COMポート
                            │
                            └── Arduino
```

Arduino CLIは、ボードやライブラリを管理し、スケッチをコンパイルしてArduinoへ書き込むための公式ツールです。

Zedの内蔵ターミナルは、`Ctrl + `` で開けます。

---

## Arduino CLIをインストールする

### Arduino CLIをダウンロードする

Arduinoの公式サイトでは、Windows向けのビルド済みArduino CLIが配布されています。

Windowsでは、インストールスクリプトが使う`sh`を標準では利用できないことがあります。その場合は、Windows用バイナリを直接ダウンロードします。

Arduino公式のインストールページから、Windows 64bit版をダウンロードして展開します。

[Arduino CLI Installation](https://docs.arduino.cc/arduino-cli/installation/)

配置場所は任意ですが、ここでは次のディレクトリを使います。

```text
C:\Tools\arduino-cli\
```

へ配置します。

```text
C:\Tools\arduino-cli\
└── arduino-cli.exe
```

---

### `arduino-cli.exe` をPATHに追加する

どのディレクトリからでも`arduino-cli`を実行できるように、

```text
C:\Tools\arduino-cli
```

をWindowsの`PATH`環境変数に追加します。

PATHを変更したら、Zedを再起動してください。

---

## `arduino-cli` が使えることを確認する

Zedのターミナルを

```text
Ctrl + `
```

で開きます。

PowerShellで次を実行します。

```powershell
arduino-cli version
```

バージョン情報が表示されれば、Arduino CLIを使える状態です。

---

## ボード情報を取得する

初回は、ボードパッケージのインデックスを取得します。

```powershell
arduino-cli core update-index
```

---

## Arduinoを接続してCOMポートを確認する

ArduinoをUSBケーブルでWindows PCに接続します。

接続後、次を実行します。

```powershell
arduino-cli board list
```

Arduino Unoなら、次のような情報が表示されます。

```text
Port   Protocol Type              Board Name    FQBN
COM3   serial   Serial Port (USB) Arduino Uno   arduino:avr:uno
```

確認するのは、次の2つです。

```text
COM3
```

`COM3`はArduinoが接続されているシリアルポートです。`arduino:avr:uno`はFQBN（Fully Qualified Board Name）と呼ばれるボード識別子です。

FQBNは、Arduino CLIにコンパイル対象のボードを伝えるために使います。

実際のCOM番号はPCによって異なります。

---

## Arduino Uno用のCoreをインストールする

Arduino UnoではAVR Coreを使います。

```powershell
arduino-cli core install arduino:avr
```

Coreには、ボード向けのコンパイラ設定、ライブラリ、書き込みツールなどが含まれます。

`core install`は、指定したCoreと必要なツールをインストールするコマンドです。

インストール状況は、

```powershell
arduino-cli core list
```

で確認できます。

Arduino Unoなら、

```text
arduino:avr
```

が表示されれば準備完了です。

---

## スケッチを作成する

新しいスケッチは、Arduino CLIのコマンドで作成できます。

作業用ディレクトリへ移動し、

```powershell
cd C:\Users\<ユーザー名>\Documents
```

```powershell
arduino-cli sketch new BlinkTest
```

を実行します。

`sketch new`は、Arduino CLIでスケッチを作成するコマンドです。

次の構成が作られます。

```text
BlinkTest/
└── BlinkTest.ino
```

Arduinoでは、**スケッチのフォルダ名とメインの`.ino`ファイル名を一致させます**。

この構成は正しい形です。

```text
BlinkTest/
└── BlinkTest.ino
```

一方、

```text
BlinkTest/
└── main.ino
```

この構成はArduinoのPrimary Sketch Fileの規則に合いません。

ArduinoのSketch Specificationでも、スケッチにはルートフォルダと同じ名前の`.ino`ファイルが必要とされています。

---

## スケッチをZedで開く

Zedで次のフォルダを開きます。

```text
File
↓
Open Folder
↓
BlinkTest
```

Zed CLIにPATHが通っている場合は、PowerShellから次のように開けます。

```powershell
zed BlinkTest
```

Windows版ZedにはCLIが含まれています。

スケッチの構成は、次のようになります。

```text
BlinkTest/
└── BlinkTest.ino
```

この形になっていれば問題ありません。

---

## LEDを点滅させるスケッチを書く

`BlinkTest.ino`をZedで開きます。

まずは、Arduino Uno本体のLEDを1秒間隔で点滅させます。

```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);

    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
```

`setup()`はArduinoの起動時に1回だけ実行されます。

```cpp
void setup()
```

その後、`loop()`が繰り返し実行されます。

```cpp
void loop()
```

---

## スケッチをコンパイルする

Zedのターミナルで、スケッチのディレクトリへ移動します。

```powershell
cd C:\Users\<ユーザー名>\Documents\BlinkTest
```

対象をArduino Unoに指定してコンパイルします。

```powershell
arduino-cli compile --fqbn arduino:avr:uno .
```

`--fqbn arduino:avr:uno`はArduino Uno向けの指定です。

```text
arduino:avr:uno
```

末尾の

```text
.
```

`.`は、現在のディレクトリにあるスケッチを対象にする指定です。

成功すると、使用したFlashメモリやRAMなどの情報が表示されます。

---

## コンパイルしたスケッチをArduinoへ書き込む

先にArduinoのCOMポートを確認します。

```powershell
arduino-cli board list
```

COMポートが`COM3`なら、

```text
COM3
```

ここで`COM3`と表示された場合は、

```powershell
arduino-cli upload -p COM3 --fqbn arduino:avr:uno .
```

を実行します。

このコマンドの指定は次のとおりです。

```text
arduino-cli upload
│
├── -p COM3
│      └── 書き込み先のポート
│
├── --fqbn arduino:avr:uno
│      └── Arduino Unoを指定
│
└── .
       └── 現在のSketch
```

現在のスケッチを、指定したCOMポートのArduino Unoへ書き込みます。

書き込みが終わると、次の経路でプログラムがArduinoに届きます。

```text
Zed
↓
Arduino CLI
↓
COM3
↓
USB
↓
Arduino Uno
```

Arduino上でプログラムが動き始めます。

---

## 開発するときの基本サイクル

環境構築が終わった後は、次の流れを繰り返します。

```text
Zedでコード編集
        ↓
保存
        ↓
arduino-cli compile
        ↓
arduino-cli upload
        ↓
Arduinoで動作確認
        ↓
Zedで修正
        ↓
再コンパイル
        ↓
再アップロード
```

普段使うコマンドは、基本的に次の2つです。

### コンパイル

```powershell
arduino-cli compile --fqbn arduino:avr:uno .
```

### 書き込み

```powershell
arduino-cli upload -p COM3 --fqbn arduino:avr:uno .
```

COM番号は次で確認できます。

```powershell
arduino-cli board list
```

Arduinoを抜き差しした後は、書き込み前に確認してください。

---

## シリアル通信を確認する

Arduino側で、次のようにシリアル出力を追加します。

```cpp
void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Hello from Arduino!");
    delay(1000);
}
```

コンパイルと書き込みの後、Arduino CLIのSerial Monitorで出力を確認できます。

```powershell
arduino-cli monitor -p COM3
```

通信速度を指定する場合は、

```powershell
arduino-cli monitor -p COM3 --config baudrate=9600
```

を実行します。

Serial Monitorには`monitor`コマンドを使います。ただし、用途によっては機能が限られるため、必要に応じて別のモニターツールを使ってください。

終了するには、

```text
Ctrl + C
```

を押します。

---

## ボードが`Unknown`になるとき

```powershell
arduino-cli board list
```

を実行して、

```text
Unknown
```

と表示されても、書き込みまで失敗するとは限りません。

ボードを自動判別できなくても、正しいFQBNとCOMポートが分かっていれば書き込める場合があります。

例えばArduino Unoを検索するなら、

```powershell
arduino-cli board listall uno
```

などで候補を確認します。

Unoの場合、

```text
arduino:avr:uno
```

を指定します。

Arduino Nano・Uno・Megaの互換ボードでは、FTDIやCH340などのUSB-Serial変換チップが使われていることがあります。その場合、Arduino CLIがボードを自動識別できないことがあります。

---

## よく使うコマンドをまとめて確認する

初回セットアップ：

```powershell
arduino-cli version

arduino-cli core update-index

arduino-cli board list

arduino-cli core install arduino:avr

arduino-cli core list
```

プロジェクト作成：

```powershell
arduino-cli sketch new BlinkTest

cd BlinkTest
```

コンパイル：

```powershell
arduino-cli compile --fqbn arduino:avr:uno .
```

Arduinoへ書き込み：

```powershell
arduino-cli board list

arduino-cli upload -p COM3 --fqbn arduino:avr:uno .
```

シリアルモニター：

```powershell
arduino-cli monitor -p COM3 --config baudrate=9600
```

---

## ツールの役割を確認する

| ツール | 役割 |
|---|---|
| Zed | Arduinoコードの編集 |
| Zed Terminal | Arduino CLIの実行 |
| Arduino CLI | Core管理・コンパイル・書き込み |
| Arduino Core | 各ボード向けのビルド環境 |
| USB / COM Port | PCとArduinoの通信 |
| Arduino | プログラムの実行 |

最終的な開発環境は次のようになります。

```text
Windows
│
├── Zed
│    ├── BlinkTest.ino
│    └── Terminal
│
├── Arduino CLI
│    ├── arduino-cli compile
│    └── arduino-cli upload
│
└── USB
     │
     └── Arduino Uno
```

この構成で、ZedからArduinoの編集・コンパイル・書き込みを一通り行えます。

---

## 参考資料

Arduino公式:

- [Arduino CLI Installation](https://docs.arduino.cc/arduino-cli/installation/)
- [Arduino CLI Getting Started](https://docs.arduino.cc/arduino-cli/getting-started/)
- [Arduino CLI Commands](https://arduino.github.io/arduino-cli/latest/commands/)
- [Arduino Sketch Specification](https://docs.arduino.cc/arduino-cli/sketch-specification/)

Zed公式:

- [Zed Documentation](https://zed.dev/docs)
