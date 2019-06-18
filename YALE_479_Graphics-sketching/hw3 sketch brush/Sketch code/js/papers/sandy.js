// A simple procedural paper that creats a sandy paper and apply the sandy effect
// on the strokes
function sandy()
{
  this.init();
}

sandy.prototype =
{
  init: function()
  {
  },

  destroy: function()
  {
  },

  apply: function(stroke, paper)
  {
    console.log("sandy apply ", stroke, "," , paper);
    if (paper)
    { // create a temporary canvas in case the bgCanvas is tainted
      var tcanvas = document.createElement("canvas");
      tcanvas.width = screenWidth;
      tcanvas.height = screenHeight;
      var tcontext = tcanvas.getContext("2d");
      
      var imgData = tcontext.getImageData(0, 0, screenWidth, screenHeight);
      var pixels = imgData.data;      
      for (var i = 0; i < pixels.length; i+=4)
      {
        var c = (Math.random()*20 - 10) + 240;
        pixels[i+0] = c;   //r
        pixels[i+1] = c;   //g
        pixels[i+2] = c;   //b
        pixels[i+3] = 255; //a
      }
      tcontext.putImageData(imgData, 0, 0);

      bgContext.clearRect(0, 0, screenWidth, screenHeight);  
      bgContext.drawImage(tcanvas, 0, 0);
    }
    if (stroke)
    { // apply the sandy effect on the strokes. This section need to be executed
      // after the sandy paper (stored in bgCanvas) is generated
      var bg_pixels = bgContext.getImageData(0, 0, screenWidth, screenHeight).data;
      var strokes = strokeContext.getImageData(0, 0, screenWidth, screenHeight);
      var pixels = strokes.data;
      
      for (var j = 0; j < screenHeight; ++j)
      {
        for (var i = 0; i < screenWidth; ++i) 
        {            
          var idx = (j*screenWidth + i)*4;
          if (pixels[idx + 3] > 0) {
            var diff = (bg_pixels[idx]-240)*4;
          
            pixels[idx+0] += diff;
            pixels[idx+1] += diff;
            pixels[idx+2] += diff;
          }
        }
      }
      context.putImageData(strokes, 0, 0);
    }    
  }  
};
