**Considering to rewrite this project by using [JUCE][https://github.com/julianstorer/JUCE] instead of wdl-ol**

# About this repository
This is an 8-bit sound waveform generator/synthesizer that I long wish to build.
I expect this audio plugin should be simple yet powerful to generate 8-bit sound (like Famicom but no just Famicom).

This is my first time writing an audio plugin,
so obviously this is my first project related to audio stuff.
I'm still learning how to make audio plugins from this tutorial:

[Making Audio Plugins by Martin Finke][Tutorial]

The whole project is base on the WDL-OL library which you can find it here:

[https://github.com/olilarkin/wdl-ol][Dependencies]

I also used these free tools to help me finishing this work:

* JKnobMan - creating knobs and faders etc.: [http://www.g200kg.com/en/software/knobman.html][Tools-JKnobMan]
* SAVIHost - testing VST version plugins: [http://www.hermannseib.com/english/savihost.htm][Tools-SAVIHost]

# How to use this repository
**Please make sure you have Visual Studio 2015 installed on your computer**

1. Clone wdl-ol repository to you local disk then checkout `vs2015x` branch
2. Follow the steps described in `wdl-ol/XXXX_SDK/readme.txt` then configure one or more SDKs you want.
3. Clone this repository under `wdl-ol/IPuginExamples/` folder
4. If you want to test VST2 and/or VST3 build, download corresponding versions of SAVIhost from link above,
copy executable file(s) to `ChipRhythm/` folder, then rename `savihost.exe` to `savihost.<vst2|vst3>.<x86|x64>.exe`

# License
MIT License

# 关于此Repo
想过自己编写一个软件合成器来着，不过嘛，一直都还没实践起来。
因为我手头上暂时还没有一个用起来顺手的8-Bit音源，于是打算写一个。

当然我在网上扒过一些8-Bit音源，例如 [Magic 8bit Plug](http://www.ymck.net/en/download/magical8bitplug/)
, 还有 [Plogue Chipsound](http://www.plogue.com/products/chipsounds/)。
Plogue Chipsound 其实挺强大的，不过大概是我当时（大约三年前，水平当然也是菜成狗）too yong too naive，
所以不怎么会用这这个音源。现在嘛，<del>大约</del>自己有一些能力，不如干脆自己写一个简单的8-Bit音源。
终于可以按照自己的意愿来操控<del>（瞎JB弄）</del>合成器，想想还是有点小激动呢！
然后顺带 C++ 练练手涨涨姿势好了<del>（乱点技能树）</del>。

实际上，我也是跟着别人入坑的。对对对，就是这个人[@BLumia](https://github.com/BLumia)。
我们也正在一起写另外一个合成器 PinappleSynth，不过我现在在弄自己这边所以就没弄那边了。
PineappleSynth 的 repo 已经挂出来了，点一下那个人的名称就能看到了。

* 有教程嘛？[戳我][Tutorial]
* 需要啥依赖库？[继续戳我][Dependencies]
* 还需要啥工具吗？[JKnobMan][Tools-JKnobMan] / [SAVIHost][Tools-SAVIHost]

# 如何使用这个Repo
<del>啊呀呀！我懒癌发作了，不写了...</del>

**请确保你电脑上安装了 Visual Studio 2015 哦亲**

1. 下载 wdl-ol 库的 `vs2015x` 分支然后解压到某个地方，或者省事点的直接走一发 `git clone`，然后切换到 `vs2015x` 分支下
2. 请阅读 `wdl-ol/XXX_SDK/` 文件夹里面十分有爱的 `readme.txt` 跟着里面的说明进行配置就好
3. 下载这个 repo，或者 `git clone` 这个 repo 到 `wdl-ol/IPluginExamples/` 文件夹下面
4. 如果需要调试 VST2 或者 VST3，跟着上面的链接下载对应的 SAVIHost。
把下载好的　`savihost.exe` 复制到 `ChipRhythm/` 文件夹下面（也就是这个repo啦），然后重命名成 `savihost.<vst2|vst3>.<x86|x64>.exe`  就可以了。
例如你需要调试 x86 平台的 VST2，下载 SAVIHost VST2 x86，复制到 `ChipRhythm/` 文件夹下面，然后重命名成
savihost.vst2.x86.exe 即可

# 许可协议
MIT！MIT！MIT！重要事情说三遍。

# 最后
不要脸的求个小星星，嘻嘻！

<del>顺带要是有哪位看上我这个单身狗，可以把我和这个 repo 一起打包带走哈！</del>

[Tutorial]: http://www.martin-finke.de/blog/tags/making_audio_plugins.html
[Dependencies]: https://github.com/olilarkin/wdl-ol
[Tools-JKnobMan]: http://www.g200kg.com/en/software/knobman.html
[Tools-SAVIHost]: http://www.hermannseib.com/english/savihost.htm


