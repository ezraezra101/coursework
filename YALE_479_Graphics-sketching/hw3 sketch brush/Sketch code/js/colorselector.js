function ColorSelector( gradient )
{
  this.init( gradient );
}

function Palette()
{
  var radius = 90;
  var count = 1080;
  var oneDivCount = 1 / count;
  var countDiv360 = count / 360;
  var degreesToRadians = Math.PI / 180;

  var canvas = document.createElement("canvas");
  canvas.width = 250;
  canvas.height = 250;

  var offsetx = canvas.width / 2;
  var offsety = canvas.height / 2;

  var context = canvas.getContext("2d");
  context.lineWidth = 1;

  for(i = 0; i < count; i++)
  {
    var angle = i / countDiv360 * degreesToRadians;
    var angle_cos = Math.cos(angle);
    var angle_sin = Math.sin(angle);

    context.strokeStyle = "hsl(" + Math.floor( (i * oneDivCount) * 360 ) + ", 100%, 50%)";
    context.beginPath();
    context.moveTo(angle_cos + offsetx, angle_sin + offsety);
    context.lineTo(angle_cos * radius + offsetx, angle_sin * radius + offsety);
    context.stroke();
  }

  var gradient = context.createRadialGradient(offsetx, offsetx, 0, offsetx, offsetx, radius);
  gradient.addColorStop(0, 'rgba(255, 255, 255, 1)');
  gradient.addColorStop(1, 'rgba(255, 255, 255, 0)');

  context.fillStyle = gradient;
  context.fillRect(0, 0, canvas.width, canvas.height);

  return canvas;
}

function HSB2RGB(hue, sat, val)
{
  if (val == 0)
    return [ 0, 0, 0 ];

  hue *= 0.016666667; // /= 60;
  sat *= 0.01; // /= 100;
  val *= 0.01; // /= 100;

  var i = Math.floor(hue);
  var f = hue - i;
  var p = val * (1 - sat);
  var q = val * (1 - (sat * f));
  var t = val * (1 - (sat * (1 - f)));

  var red, green, blue;
  switch(i)
  {
    case 0: red = val; green = t; blue = p; break;
    case 1: red = q; green = val; blue = p; break;
    case 2: red = p; green = val; blue = t; break;
    case 3: red = p; green = q; blue = val; break;
    case 4: red = t; green = p; blue = val; break;
    case 5: red = val; green = p; blue = q; break;
  }

  return [red, green, blue];
}

function RGB2HSB(red, green, blue)
{
  var x = Math.min( Math.min( red, green ), blue );
  var val = Math.max( Math.max( red, green ), blue );

  if (x==val)
    return [0, 0, val*100];

  var f = (red == x) ? green - blue : ((green == x) ? blue - red : red - green);
  var i = (red == x) ? 3 : ((green == x) ? 5 : 1);

  var hue = Math.floor((i - f / (val - x)) * 60) % 360;
  var sat = Math.floor(((val - x) / val) * 100);
  var val = Math.floor(val * 100);

  return [hue, sat, val];
}

ColorSelector.prototype =
{
  container: null,
  color: [0, 0, 0],

  hueSelector: null,
  luminosity: null,
  luminosityData: null,
  luminositySelector: null,
  luminosityPosition: null,

  dispatcher: null,
  changeEvent: null,

  init: function(gradient)
  {
    this.container = document.createElement('div');
    this.container.style.position = 'absolute';
    this.container.style.width = '250px';
    this.container.style.height = '250px';
    this.container.style.visibility = 'hidden';
    this.container.style.cursor = 'pointer';
    this.container.addEventListener('mousedown', onMouseDown, false);
    this.container.addEventListener('touchstart', onTouchStart, false);

    var hue = document.createElement("canvas");
    hue.width = gradient.width;
    hue.height = gradient.height;

    var context = hue.getContext("2d");
    context.drawImage(gradient, 0, 0, hue.width, hue.height);

    var hueData = context.getImageData(0, 0, hue.width, hue.height).data;

    this.container.appendChild(hue);

    this.luminosity = document.createElement("canvas");
    this.luminosity.style.position = 'absolute';
    this.luminosity.style.left = '0px';
    this.luminosity.style.top = '0px';
    this.luminosity.width = 250;
    this.luminosity.height = 250;

    this.container.appendChild(this.luminosity);

    this.hueSelector = document.createElement("canvas");
    this.hueSelector.style.position = 'absolute';
    this.hueSelector.style.left = ((hue.width - 15) / 2 ) + 'px';
    this.hueSelector.style.top = ((hue.height - 15) / 2 ) + 'px';
    this.hueSelector.width = 15;
    this.hueSelector.height = 15;

    context = this.hueSelector.getContext("2d");
    context.lineWidth = 2;
    context.strokeStyle = "rgba(0, 0, 0, 0.5)";
    context.beginPath();
    context.arc(8, 8, 6, 0, Math.PI * 2, true);
    context.stroke();
    context.strokeStyle = "rgba(256, 256, 256, 0.8)";
    context.beginPath();
    context.arc(7, 7, 6, 0, Math.PI * 2, true);
    context.stroke();

    this.container.appendChild( this.hueSelector );

    this.luminosityPosition = [ (gradient.width - 15), (gradient.height - 15) / 2 ];

    this.luminositySelector = document.createElement("canvas");
    this.luminositySelector.style.position = 'absolute';
    this.luminositySelector.style.left = (this.luminosityPosition[0] - 7) + 'px';
    this.luminositySelector.style.top = (this.luminosityPosition[1] - 7) + 'px';
    this.luminositySelector.width = 15;
    this.luminositySelector.height = 15;

    context = this.luminositySelector.getContext("2d");
    context.drawImage(this.hueSelector, 0, 0, this.luminositySelector.width, this.luminositySelector.height);

    this.container.appendChild(this.luminositySelector);

    this.dispatcher = document.createElement('div');

    this.changeEvent = document.createEvent('Events');
    this.changeEvent.initEvent('change', true, true);

    var scope = this;

    function onMouseDown( event )
    {
      window.addEventListener('mousemove', onMouseMove, false);
      window.addEventListener('mouseup', onMouseUp, false);

      update( event.clientX - scope.container.offsetLeft, event.clientY - scope.container.offsetTop );
    }

    function onMouseMove( event )
    {
      update( event.clientX - scope.container.offsetLeft, event.clientY - scope.container.offsetTop );
    }

    function onMouseUp( event )
    {
      window.removeEventListener('mousemove', onMouseMove, false);
      window.removeEventListener('mouseup', onMouseUp, false);

      update( event.clientX - scope.container.offsetLeft, event.clientY - scope.container.offsetTop );
    }

    function onTouchStart( event )
    {
      if(event.touches.length == 1)
      {
        event.preventDefault();

        window.addEventListener('touchmove', onTouchMove, false);
        window.addEventListener('touchend', onTouchEnd, false);

        update( event.touches[0].pageX - scope.container.offsetLeft, event.touches[0].pageY - scope.container.offsetTop );
      }
    }

    function onTouchMove( event )
    {
      if(event.touches.length == 1)
      {
        event.preventDefault();

        update( event.touches[0].pageX - scope.container.offsetLeft, event.touches[0].pageY - scope.container.offsetTop );
      }
    }

    function onTouchEnd( event )
    {
      if(event.touches.length == 0)
      {
        event.preventDefault();

        window.removeEventListener('touchmove', onTouchMove, false);
        window.removeEventListener('touchend', onTouchEnd, false);
      }
    }

    function update(x, y)
    {
      var dx = x - 125;
      var dy = y - 125;
      var d = Math.sqrt( dx * dx + dy * dy );

      if (d < 90)
      {
        scope.hueSelector.style.left = (x - 7) + 'px';
        scope.hueSelector.style.top = (y - 7) + 'px';
        scope.updateLuminosity( [ hueData[(x + (y * 250)) * 4], hueData[(x + (y * 250)) * 4 + 1], hueData[(x + (y * 250)) * 4 + 2] ] );
      }
      else if (d > 100)
      {
        var nx = dx / d;
        var ny = dy / d;

        scope.luminosityPosition[0] = (nx * 110) + 125;
        scope.luminosityPosition[1] = (ny * 110) + 125;

        scope.luminositySelector.style.left = ( scope.luminosityPosition[0] - 7) + 'px';
        scope.luminositySelector.style.top = ( scope.luminosityPosition[1] - 7) + 'px';
      }

      x = Math.floor(scope.luminosityPosition[0]);
      y = Math.floor(scope.luminosityPosition[1]);

      scope.color[0] = scope.luminosityData[(x + (y * 250)) * 4];
      scope.color[1] = scope.luminosityData[(x + (y * 250)) * 4 + 1];
      scope.color[2] = scope.luminosityData[(x + (y * 250)) * 4 + 2];

      scope.dispatchEvent( scope.changeEvent );
    }
  },

  show: function()
  {
    this.container.style.visibility = 'visible';
  },

  hide: function()
  {
    this.container.style.visibility = 'hidden';
  },

  getColor: function()
  {
    return this.color;
  },

  setColor: function( color )
  {
    var degreesToRadians = Math.PI / 180;

    this.color = color;

    var hsb = RGB2HSB(color[0] / 255, color[1] / 255, color[2] / 255);

    var angle = hsb[0] * degreesToRadians;
    var distance = (hsb[1] / 100) * 90;

    this.hueSelector.style.left = ( ( Math.cos(angle) * distance + 125 ) - 7 ) + 'px';
    this.hueSelector.style.top = ( ( Math.sin(angle) * distance + 125 ) - 7 ) + 'px';

    var rgb = HSB2RGB(hsb[0], hsb[1], 100);
    rgb[0] *= 255; rgb[1] *= 255; rgb[2] *= 255;

    this.updateLuminosity( rgb );

    angle = (hsb[2] / 100) * 360 * degreesToRadians;

    this.luminosityPosition[0] = ( Math.cos(angle) * 110 ) + 125;
    this.luminosityPosition[1] = ( Math.sin(angle) * 110 ) + 125;

    this.luminositySelector.style.left = ( this.luminosityPosition[0] - 7 ) + 'px';
    this.luminositySelector.style.top = ( this.luminosityPosition[1] - 7 ) + 'px';

    this.dispatchEvent( this.changeEvent );
  },

  updateLuminosity: function( color )
  {
    var inner_radius = 100;
    var outter_radius = 120;
    var count = 1080 / 2;
    var oneDivCount = 1 / count;
    var degreesToRadians = Math.PI / 180;
    var countDiv360 = (count / 360);

    var offsetx = this.luminosity.width / 2;
    var offsety = this.luminosity.height / 2;

    var context = this.luminosity.getContext("2d");
    context.lineWidth = 3;
    context.clearRect(0, 0, this.luminosity.width, this.luminosity.height);

    for(var i = 0; i < count; i++)
    {
      var angle = i / countDiv360 * degreesToRadians;
      var angle_cos = Math.cos(angle);
      var angle_sin = Math.sin(angle);

      var shade = 255 - (i * oneDivCount /* / count */) * 255;

      context.strokeStyle = "rgb(" + Math.floor( color[0] - shade ) + "," + Math.floor( color[1] - shade ) + "," + Math.floor( color[2] - shade ) + ")";
      context.beginPath();
      context.moveTo(angle_cos * inner_radius + offsetx, angle_sin * inner_radius + offsety);
      context.lineTo(angle_cos * outter_radius + offsetx, angle_sin * outter_radius + offsety);
      context.stroke();
    }

    this.luminosityData = context.getImageData(0, 0, this.luminosity.width, this.luminosity.height).data;
  },

  addEventListener: function( type, listener, useCapture )
  {
    this.dispatcher.addEventListener(type, listener, useCapture);
  },

  dispatchEvent: function( event )
  {
    this.dispatcher.dispatchEvent(event);
  },

  removeEventListener: function( type, listener, useCapture )
  {
    this.dispatcher.removeEventListener(type, listener, useCapture);
  }
}
