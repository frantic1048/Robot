# Robot

C++ 作业，模拟机器人巡逻。

所有机器人逻辑由 C++ 编写成，通过 [Emscripten](http://emscripten.org/) 编译到 [asm.js](http://asmjs.org/) 供调用。在 [Electron](http://electron.atom.io/) 中实现图形显示。

# 构建

环境需求

- Emscripten
- Node.js/io.js
- npm

安装依赖的 node 模块（在此仓库的根目录执行，构建也是）

    $npm install

开始构建

    $gulp

# 运行

##Electron

    $electron app

##现代浏览器

访问：[https://frantic1048.github.com/Robot/](https://frantic1048.github.com/Robot/)
