    <head>
        <meta charset='utf-8'>
        <title>图片加载平移放大缩小示例</title>
        <style>
            html,body{margin:10px;
                    padding:10px;}
            canvas{border: 1px solid #000;
                margin-left: 2px;
                margin-right: 2px;}
            pre{background: #aaa;}
        </style>
        <script src="./node_modules/image_zoom/image_zoom3.js"></script>
        <script src="./node_modules/html-table-of-contents/src/html-table-of-contents.js">        </script>
        <link rel="stylesheet"   type="text/css"
                href="./node_modules/html-table-of-contents/html-table-of-contents.css" />
        <script src="load_mathjax.js" async></script>
    </head>
<body onLoad="htmlTableOfContents(); drawAllTables();">
<div style="font-size: 2em; text-align: center; font-weight: bold;">使用image_zoom做汇报方案</div>
<p><b>Contents</b> </p>
<div id="toc">        </div>
<!-- 如何使用html-table-of-contents请参考这里：https://www.npmjs.com/package/html-table-of-contents -->


# image zoom是什么
image_zoom是一个基于html的简单框架，用于带有简单图像比较的文档展示。例如，当我们进行一个实验时，我们往往需要写一些文字，同时展示一些结果，
有时还需要将两种不同方法的结果进行比较，包括放大，拖拽，等待。这种有交互需求的展示，用word或PPT都是很不方便，因此我写了这样一个工具，来使用
html写文档。<br/>
在需要比较两个图片的地方，只需要把下面这段代码嵌入html网页即可。注意要把图片的路径改成你的真实图片路径。<br/>
在body的onload函数中写了drawAllTables();这个函数用来查找网页中所有的TabbleComp对象，并将图片画出来。
image_zoom主要包括2个部分：
1. html文档：在一个表格中放入了canvas和caption，用来显示图片以及标题。使用时用户需要自己根据需要修改这部分代码。<br/>
      表格的类型必须为 class="TableComp"，否则无法正常显示。id可以省略。<br/>
      可以将canvas按任意的形式排列，例如可以1x3，也可以2x2。图片和标题的大小都可以自动适应。
      排列时canvas和caption必须保持一致。
2. javascript脚本：定义了鼠标事件，包括点击图片后拖动，滚动鼠标滚轮放大/缩小图片。这部分代码是不需要用户修改的。

<table id="compare1" class="TableComp">
  <tr>
          <td>left image: <input type="text" class="caption" size=80/></td>
          <td>right image: <input type="text" class="caption" size=80/></td></tr>

  <tr><td>    <button id="btn_fit" onclick="fitImage(event);">适应页面</button>
    <button id="btn_load" onclick="loadImage(event);">重新载入</button></td>
  </tr>
  <tr>
          <td><canvas width="600" height="500" title="pics/me.jpg" data-pos="-400,-100,0.8"></canvas>
          <td><canvas width="600" height="500" title="pics/me2.jpg"></canvas></td></tr>
  <tr>
    <td>right image: <input type="text" class="caption" size=80/></td>
    <td>right image: <input type="text" class="caption" size=80/></td>
  </tr>
  <tr>
    <td><canvas width="600" height="500" title="pics/me3.jpg"></canvas>
      <td><canvas width="600" height="500" title="pics/me2.jpg"></canvas>
  </tr>
  <tr><td class="PoseInfo">pos and scale</td>        </tr>
</table>

## 各参数的含义
1. canvas,id: 画布相关参数，不可更改；
2. width,height: 图像的宽高。其中宽度会跟据窗口宽度调节，也不用改。高度跟要对比的图像高度有关，可以修改
3. title: 图像路径
4. data-imgX, data-imgY, data-imgScale: 对比图像的初始位置及尺度

# image_zoom如何使用
1. **放大/缩小**：鼠标放到左边的图上，滚轮向前/向后滚动即可。注意，此时网页的滚动是被禁止的。如果要恢复网页的滚动，      只需要把鼠标从左图的区域移开即可。
2. **同步拖动**: 鼠标放到左图区域上，按下左键不要放开，拖动图片即可。
3. <b>拖动单张图</b>：鼠标放到指定图区域上，按下右键不要放开，拖动图片即可。
4. <b>设置初始位置及尺度</b>：设置canvas的data-imgX, data-imgY, data-imgScale这三个属性。
5. <b>如何确定这三个属性应该怎么设置？</b>
      当我们拖动或缩放图片时，表格的最后一行，<b>image pos and scale</b>，会实时显示当前的imgX,imgY,imgScale这3个值。
6. <b>交换左右图：</b>shift+左键点击某个图片即可交换当前图与下一张图（如果当前图是最后一张，则交换当前图与第一张图），放开左键还原
7. <b>适应窗口大小：</b>点击“适应页面”按钮可使所有图适应窗口大小

# 使用markdown或html语言
这个工具原先是用html+JS开发的，但后来发现用markdown可能更友好，毕竟我们并不关心html的一些花哨的功能，只是把它当作一个“带有图片缩放和拖动功能的文本编辑器”。markdown可以完美兼容大部分html标签，不能兼容的那些，经过VS Code的"Markdown All in One"转换为html，也都可以正常显示。

因此，我们一样可以在文档的头部写上html格式的`<head>`标签，把需要加载的JS脚本都写上。接下来也可以写`<body>`标签，写上`onload`函数。这样当markdown转换为html后，就是正常的网页了。

# 插入公式
可以使用MathJax来插入公式。  首先要在head中插入load_math.js，其内容是这样的：
<p>
  <pre>
    window.MathJax = {
      tex: {
        inlineMath: [['$', '$'], ['\\(', '\\)']]
      },
      svg: {
        fontCache: 'global'
      }
    };

    (function () {
      var script = document.createElement('script');
      script.src = './node_modules/mathjax/es5/tex-chtml.js';
      script.async = true;
      document.head.appendChild(script);
    })();
  </pre>
  </p>
<p>  然后在需要写入公式的地方插入Latex代码即可。例如这样：
<pre>$y=a^2+\sin{\pi/2}$</pre>
显示为公式：
  $y=a^2+\sin{\pi/2}$
</p>

# 插入表格
可以用html或者markdown风格插入表格。如果用markdown风格就是这样：（注意表格需要与前面的文字空一行）。

## markdown表格
- 使用 | 来分隔不同的单元格，使用 - 来分隔表头和其他行，
- 在表头下方的分隔线标记中加入 :，即可标记下方单元格内容的对齐方式：
  - :--- 代表左对齐
  - :--: 代表居中对齐
  - ---: 代表右对齐

| 表头与其他内容用短横线隔开 | 姓名 | 年龄 |
| :--   | :--:   | --:  |
| 序号1| 张三 | 23  |
|序号2 | 李四 | 23  |

## html表格
直接插入`<table>`标签即可，跟普通的html写法一样。
<table border="1">
    <caption>
    表格示例
    </caption>
    <tr>
    <td width="60">方法</td>
    <td width="134">方法一</td>
    <td width="142">方法二</td>
    </tr>
    <tr>
    <td>PSNR</td>
    <td>23.5</td>
    <td>24.7</td>
    </tr>
    <tr>
    <td>SSIM</td>
    <td>0.8</td>
    <td>0.9</td>
    </tr>
</table>
</body>