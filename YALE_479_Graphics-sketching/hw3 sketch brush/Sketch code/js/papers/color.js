// A simple paper of soild color
function color()
{
  this.init();
}

color.prototype =
{
  init: function()
  {
  },

  destroy: function()
  {
  },

  apply: function(stroke, paper)
  {
    if (stroke) 
    { // there is no paper effect of this paper, so we will just copy the strokes over
      context.clearRect(0, 0, screenWidth, screenHeight);  
      context.drawImage(strokeCanvas, 0, 0);
    }
    if (paper) 
    { // fill the paper with the selected canvas color
      bgContext.fillStyle = 'rgb(' + CANVAS_COLOR[0] + ', ' + CANVAS_COLOR[1] + ', ' + CANVAS_COLOR[2] + ')';
      bgContext.fillRect(0, 0, screenWidth, screenHeight);        
    }
  }
};
