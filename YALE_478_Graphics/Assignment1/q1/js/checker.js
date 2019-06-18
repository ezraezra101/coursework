function doStuff()
{

var c = document.getElementById("myCanvas");
var ctx = c.getContext("2d");
var imgWidth = 500;
var imgHeight = 500;
var nsquares = document.getElementById("num_squares").value;

 var  oneco = [255, 255, 255];
 oneco[0] = document.getElementById("oner").value;
 oneco[1] = document.getElementById("oneg").value;
 oneco[2] = document.getElementById("oneb").value;
 var twoco = [255, 255, 255];
 twoco[0] = document.getElementById("twor").value;
 twoco[1] = document.getElementById("twog").value;
 twoco[2] = document.getElementById("twob").value;
var imgData = ctx.createImageData(imgWidth, imgHeight);

// modify this section to make checkerboard
// default image is one gray square

for (jj = 0; jj < imgWidth; jj++)
{ var qq = jj*4;
	for(ii = 0; ii < imgHeight; ii += 1)
{
  var pp = (ii*imgWidth*4)+qq;
	imgData.data[pp] = 128;
	imgData.data[pp+1] = 128;
	imgData.data[pp+2] = 128;	
	imgData.data[pp+3]=255;
  
}
}


ctx.putImageData(imgData, 20, 20);
}
