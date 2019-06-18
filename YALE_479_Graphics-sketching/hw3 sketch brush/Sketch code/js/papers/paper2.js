
// A simple paper of soild color
function paper2()
{
  this.init();
}

paper2.prototype =
{

  noise: null,

  init: function()
  {
    this.noise = new Noise(4,0.05);
  },

  destroy: function()
  {
  },

  apply: function(stroke, paper)
  {
    if (stroke) 
    { // there is no paper effect of this paper, so we will just copy the strokes over
      context.clearRect(0, 0, screenWidth, screenHeight);
      context.scale(1,1);
      context.drawImage(strokeCanvas, 0, 0);
      context.scale(-1,1);
      context.drawImage(strokeCanvas, -screenWidth, 0);
      context.scale(1,1);

    }
    if (paper)
    { // create a temporary canvas in case the bgCanvas is tainted
      var tcanvas = document.createElement("canvas");
      tcanvas.width = screenWidth;
      tcanvas.height = screenHeight;
      var tcontext = tcanvas.getContext("2d");
      
      var imgData = tcontext.getImageData(0, 0, screenWidth, screenHeight);
      var pixels = imgData.data;      
      for (var i = 0; i < pixels.length/4; i++)
      {
        var y = Math.floor(i / tcanvas.width);
        var x = i-y*tcanvas.width;
        var c = 240+10*this.noise.noise(x, y);
        pixels[4*i+0] = x == Math.floor(tcanvas.width/2) ? 0 : c;   //r
        pixels[4*i+1] = c;   //g
        pixels[4*i+2] = c;   //b
        pixels[4*i+3] = 255; //a
      }
      tcontext.putImageData(imgData, 0, 0);

      context.clearRect(0, 0, screenWidth, screenHeight);
      bgContext.clearRect(0, 0, screenWidth, screenHeight);  
      bgContext.drawImage(tcanvas, 0, 0);
    }
  }
};
