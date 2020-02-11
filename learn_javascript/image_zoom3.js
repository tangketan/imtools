
function drawAllCanvas(tableID){
    var table = document.getElementById(tableID)
    var canvases=table.getElementsByTagName("canvas");
    var contexts = new Array(canvases.length);
    var imgs = new Array(canvases.length);
    var captions = table.getElementsByClassName("caption");
    var posInfo = table.getElementsByClassName("PoseInfo")[0];
    for(var i=0; i< canvases.length; i++)
    {
        canvas = canvases[i];
        canvas.width = screen.width/canvases.length-50;
        contexts[i]=canvas.getContext('2d');
        canvas.tabIndex = i;
        canvas.canvases = canvases;  // add extra attributes to access all canvases
        captions[i].setAttribute("size", 160/captions.length);
        canvas.caption = captions[i];
        canvas.addEventListener("mousedown", mouse_down, false);
    }
    var canvas=canvases[0];
    var pos = canvas.dataset.pos.split(',');
    canvas.imgX = parseFloat(pos[0]); //(canvas.dataset.imgx); // note here imgx must be all lower case even if you set data-imgX in html
    canvas.imgY = parseFloat(pos[1]);//(canvas.dataset.imgy);
    canvas.imgScale = parseFloat(pos[2]);//(canvas.dataset.imgscale);
    canvas.posInfo = posInfo;
    loadImg(canvases);


    // canvas.onmousewheel=canvas.οnwheel=mouse_wheel; // both methods work
    canvas.addEventListener("wheel", mouse_wheel, false)
    canvas.onmouseleave=function(){
        document.documentElement.style.overflow='auto'; // restore page scroll behavior
    }

}

function loadImg(canvases){
    for(i=0; i< canvases.length; i++)
    {
        var img=new Image();
        if(i==canvases.length-1){
            // img.onload doesn't work
            img.addEventListener('load',function(){drawImage(canvases);} );
        }
        img.src=canvases[i].title; //图像地址由html传入
        canvases[i].img = img;
    }
}

function drawImage(canvases){
    var imgX = canvases[0].imgX;
    var imgY = canvases[0].imgY;
    var imgScale = canvases[0].imgScale;
    for(i=0; i< canvases.length; i++)
    {
        var img = canvases[i].img;
        canvases[i].getContext('2d').clearRect(0,0,canvases[i].width,canvases[i].height);
        canvases[i].getContext('2d').drawImage(img,0,0,img.width,img.height,
            imgX,imgY,img.width*imgScale,img.height*imgScale);
        canvases[i].caption.value = img.src;
    }
    canvases[0].posInfo.innerHTML = "<b>image pos and scale:</b> " + imgX.toFixed(0)+","+imgY.toFixed(0)+","+imgScale.toFixed(3);
}

function mouse_down(event){
    var canvas = event.target || event.srcElement;
    var canvases = canvas.canvases;
    if(event.button==0){
        // 左键按下拖动
        var pos=windowToCanvas(canvas,event.clientX,event.clientY);
        canvas.onmousemove=function(event){
            canvas.style.cursor="move";
            var pos1=windowToCanvas(canvas,event.clientX,event.clientY);
            var x=pos1.x-pos.x;
            var y=pos1.y-pos.y;
            pos=pos1;
            canvases[0].imgX+=x;
            canvases[0].imgY+=y;
            drawImage(canvases);
        };
        canvas.onmouseup=function(){
            canvas.onmousemove=null;
            canvas.onmouseup=null;
            canvas.style.cursor="default";
        };
    }
    else if(event.button==2){
        // 右键点击切换当前图与下一张图
        var canvas_index = canvas.tabIndex;
        switchImage(canvases, canvas_index, canvas_index+1);
        canvas.onmouseup=function(){drawImage(canvases);};
    }
   
}


function mouse_wheel(event)
{
    document.documentElement.style.overflow='hidden'; // stop page to scroll
    var canvas = event.target || event.srcElement;
    var canvases = canvas.canvases;
    var pos=windowToCanvas(canvas,event.clientX,event.clientY);
    event.wheelDelta=event.wheelDelta?event.wheelDelta:(event.deltaY*(-40));
    let scaleStep=1.2;
    if(event.wheelDelta>0){
        canvases[0].imgScale*=scaleStep;
        canvases[0].imgX=(canvases[0].imgX-pos.x)*scaleStep+pos.x;
        canvases[0].imgY=(canvases[0].imgY-pos.y)*scaleStep+pos.y;
    }else{
        canvases[0].imgScale/=scaleStep;
        canvases[0].imgX=(canvases[0].imgX-pos.x)/scaleStep+pos.x;
        canvases[0].imgY=(canvases[0].imgY-pos.y)/scaleStep+pos.y;
    }
    drawImage(canvases);
}

function windowToCanvas(canvas,x,y){
    var bbox = canvas.getBoundingClientRect();
    return {
        x:x - bbox.left - (bbox.width - canvas.width) / 2,
        y:y - bbox.top - (bbox.height - canvas.height) / 2
    };
}

function switchImage(canvases, ind0_, ind1_)
{
    ind0 = ind0_ || 0;
    ind1 = ind1_ || 1;
    ind0 = ind0 % canvases.length;
    ind1 = ind1 % canvases.length;
    var imgX = canvases[0].imgX;
    var imgY = canvases[0].imgY;
    var imgScale = canvases[0].imgScale;
    // show img2 on canvas and img on canvas2
    canvases[ind0].getContext('2d').clearRect(0,0,canvases[ind0].width,canvases[ind0].height);
    canvases[ind0].getContext('2d').drawImage(canvases[ind1].img,0,0,canvases[ind1].img.width,canvases[ind1].img.height,
        imgX,imgY,canvases[ind1].img.width*imgScale,canvases[ind1].img.height*imgScale);    
    canvases[ind1].getContext('2d').clearRect(0,0,canvases[ind1].width,canvases[ind1].height);
    canvases[ind1].getContext('2d').drawImage(canvases[ind0].img,0,0,canvases[ind0].img.width,canvases[ind0].img.height,
        imgX,imgY,canvases[ind0].img.width*imgScale,canvases[ind0].img.height*imgScale);    
}

function fitImage(event)
{
    // both methods work
//     var canvases= $(event.target).closest("table").get(0).getElementsByTagName("canvas");
    var canvases= event.target.closest("table").getElementsByTagName("canvas");
    canvases[0].imgX=0;
    canvases[0].imgY=0;
    canvases[0].imgScale = canvases[0].width/canvases[0].img.width;
    drawImage(canvases);
}

function loadImage(event)
{
//     var canvases= $(event.target).closest("table").get(0).getElementsByTagName("canvas");
    var canvases= event.target.closest("table").getElementsByTagName("canvas");
    for(i=0;i<canvases.length;i++){
        canvases[i].img.src = canvases[i].caption.value;
    }
    drawImage();
}

function randomString(len) {
　　len = len || 32;
　　var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678';    /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
　　var maxPos = $chars.length;
　　var pwd = '';
　　for (i = 0; i < len; i++) {
　　　　pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
　　}
　　return pwd;
}