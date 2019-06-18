function brush2()
{
    this.init();
}

brush2.prototype =
{
    prevX: null, prevY: null,

    points: null, count: null,
    dirs: null,

    init: function()
    {
      this.points = new Array();
      this.dirs = new Array();
      this.count = 0;
    },

    destroy: function()
    {
    },

    strokeStart: function( x, y )
    {
      this.prevX = x;
      this.prevY = y;
    },

    stroke: function( x, y)
    {
        this.points.push( [ x, y ] );
        var dist = Math.sqrt(Math.pow(x-this.prevX,2)+Math.pow(y-this.prevY,2));
        this.dirs.push( [ (x-this.prevX)/dist, (y-this.prevY)/dist] );

        strokeContext.globalCompositeOperation = 'source-over';
        strokeContext.lineWidth = BRUSH_SIZE;
        strokeContext.strokeStyle = "rgba(" + BRUSH_COLOR[0] + ", " + BRUSH_COLOR[1] + ", " + BRUSH_COLOR[2] + ", " + 0.05 * BRUSH_PRESSURE + ")";

        strokeContext.beginPath();
        strokeContext.moveTo(this.prevX, this.prevY);
        strokeContext.lineTo(x, y);
        strokeContext.stroke();

        for (var i = this.count; i > 0; i--)
        {
            var dot = this.dirs[i][0] * this.dirs[this.count][0] + this.dirs[i][1] * this.dirs[this.count][1];
            if(dot < 0.2) {
              break;
            }
            var dx = this.points[i][0] - this.points[this.count][0];
            var dy = this.points[i][1] - this.points[this.count][1];
            var d = dx * dx + dy * dy;
            var dot = this.dirs[i][0] * this.dirs[this.count][0] + this.dirs[i][1] * this.dirs[this.count][1];

            if ( Math.abs(dot) > 0.5 && d < 8000 && Math.random() > (d / 20000))
            {
                var lessLength = Math.random()*0.8+0.1;
                strokeContext.beginPath();
                strokeContext.moveTo( this.points[this.count][0] + (dx * lessLength), this.points[this.count][1] + (dy * lessLength));
                strokeContext.lineTo( this.points[i][0] - (dx * lessLength), this.points[i][1] - (dy * lessLength));
                strokeContext.stroke();
            }
        }

        this.prevX = x;
        this.prevY = y;

        this.count++;
    },

    strokeEnd: function()
    {
      this.init();
    }
}
