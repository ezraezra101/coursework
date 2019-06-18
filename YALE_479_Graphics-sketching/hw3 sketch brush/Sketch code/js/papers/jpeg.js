// A example paper of a loaded jpeg image. The image is stretched to fill the 
// canavas in this example.
function jpeg()
{
  this.init( context );
}

jpeg.prototype =
{
  texture: null,
  
  init: function()
  {
    this.texture = new Image();
    this.texture.src = 'img/canvas.jpg';
  },

  destroy: function()
  {
  },

  draw: function() {
  },
  
  apply: function(stroke, paper)
  {
    console.log("jpeg apply ", stroke, "," , paper);
    if (stroke)
    {
      context.clearRect(0, 0, screenWidth, screenHeight);  
      context.drawImage(strokeCanvas, 0, 0);        
    }
    
    if (paper)
    {
      if (this.texture.complete) 
      {
        bgContext.drawImage(this.texture, 0, 0, screenWidth, screenHeight);
      } else {
        this.texture = new Image();
        this.texture.src = 'img/canvas.jpg';
        this.texture.onload = function() {
          bgContext.drawImage(this, 0, 0, screenWidth, screenHeight); 
        };
      }
    }
  }  
};
