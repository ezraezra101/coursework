function sketchy()
{
    this.init();
}

sketchy.prototype =
{
    random: function(){return 1;},
    prevX: null, prevY: null,

    points: null, count: null,

    init: function()
    {
      this.points = new Array();
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

        strokeContext.globalCompositeOperation = 'source-over';
        strokeContext.lineWidth = BRUSH_SIZE;
        strokeContext.strokeStyle = "rgba(" + BRUSH_COLOR[0] + ", " + BRUSH_COLOR[1] + ", " + BRUSH_COLOR[2] + ", " + 0.05 * BRUSH_PRESSURE + ")";

        strokeContext.beginPath();
        strokeContext.moveTo(this.prevX, this.prevY);
        strokeContext.lineTo(x, y);
        strokeContext.stroke();

        for (var i = 0; i < this.points.length; i++)
        {
            var dx = this.points[i][0] - this.points[this.count][0];
            var dy = this.points[i][1] - this.points[this.count][1];
            var d = dx * dx + dy * dy;

            if (d < 4000 && Math.random() > (d / 2000))
            {
                strokeContext.beginPath();
                strokeContext.moveTo( this.points[this.count][0] + (dx * 0.3*this.random()), this.points[this.count][1] + (dy * 0.3*this.random()));
                strokeContext.lineTo( this.points[i][0] - (dx * 0.3*this.random()), this.points[i][1] - (dy * 0.3*this.random()));
                strokeContext.stroke();
            }
        }

        this.prevX = x;
        this.prevY = y;

        this.count++;
    },

    strokeEnd: function()
    {

    }
}
