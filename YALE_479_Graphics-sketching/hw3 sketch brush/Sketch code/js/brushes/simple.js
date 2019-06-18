function simple()
{
  this.init();
}

// This is an example of how to create a brush to work within the frame work.
// There are in general 3 steps to create a brush:
// 1. create a javascript file defining the brush. The brush file is recommended
//    to store inside the brushes directory.
//    - the brush name should be used consistently within the framework: the
//      js file name, the main function name (line 1 in the brush js), the brush
//      name in BRUSHES (defined in main.js)..etc
//    - the brush prototype has to contain these three stroke functions:
//        strokeStart(x, y)
//        stroke(x, y)
//        strokeEnd()
// 2. add this script element inside <body> in index.html. Note that {BRUSH_NAME}
//    should be the name of your new brush defined in the step 1.
//    <script type="text/javascript" src="js/brushes/{BRUSH_NAME}.js"></script>
// 3. add the name of your new brush into the variable BRUSHES in main.js
//
// Now load index.html into a html5 ready browser (if you don't know which
// one to use, Google Chrome is recommaned as it has a nice set of development
// features), you should be able to find the new brush in the drop-down list
// of the brush selector.
simple.prototype =
{
  prevX: null, prevY: null,

  init: function()
  {
  },

  destroy: function()
  {
  },

  // called when the touch is DOWN
  strokeStart: function( x, y )
  {
    this.prevX = x;
    this.prevY = y;
  },

  // called when the touch MOVEs
  stroke: function( x, y )
  {
    strokeContext.globalCompositeOperation = 'source-over';
    strokeContext.lineCap = 'round';
    strokeContext.lineWidth = BRUSH_SIZE;
    strokeContext.strokeStyle = "rgba(" + BRUSH_COLOR[0] + ", " + BRUSH_COLOR[1] + ", " + BRUSH_COLOR[2] + ", " + 0.5 * BRUSH_PRESSURE + ")";

    strokeContext.beginPath();
    strokeContext.moveTo(this.prevX, this.prevY);    
    var dist = Math.sqrt((x-this.prevX)*(x-this.prevX) + (y-this.prevY)*(y-this.prevY));
    var dt = 1/dist;
    for (var t = 0; t <= 1; t+=dt)
    {
      var xx = this.prevX * (1-t) + x * t;
      var yy = this.prevY * (1-t) + y * t;
      strokeContext.lineTo(xx, yy);
      strokeContext.stroke();
    }
    this.prevX = x;
    this.prevY = y;
  },

  // called when the touch is UP
  strokeEnd: function()
  {

  }
}
