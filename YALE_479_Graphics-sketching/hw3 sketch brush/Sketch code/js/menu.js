function Menu()
{ // generate the menu elements inside the body element in index.html
  this.init();
}

Menu.prototype =
{
  container: null,

  brushColor: null,
  canvasColor: null,

  brushSizeInc: null,
  brushSizeDec: null,

  brushSelector: null,
  paperSelector: null,
  
  save: null,
  clear: null,

  init: function()
  {
    this.container = document.createElement("div");
    this.container.className = 'gui';
    this.container.style.position = 'absolute';
    this.container.style.top = '0px';

    var color_width = 20, color_height = 15;

    this.brushColor = document.createElement("canvas");
    this.brushColor.style.marginBottom = '-3px';
    this.brushColor.style.marginRight = '5px';
    this.brushColor.style.cursor = 'pointer';
    this.brushColor.width = color_width;
    this.brushColor.height = color_height;
    this.container.appendChild(this.brushColor);

    this.setBrushColor( BRUSH_COLOR );

    var space = document.createTextNode(" ");
    this.container.appendChild(space);

    this.canvasColor = document.createElement("canvas");
    this.canvasColor.style.marginBottom = '-3px';
    this.canvasColor.style.marginRight = '10px';
    this.canvasColor.style.cursor = 'pointer';
    this.canvasColor.width = color_width;
    this.canvasColor.height = color_height;
    this.container.appendChild(this.canvasColor);

    this.setCanvasColor( CANVAS_COLOR );

    var separator = document.createTextNode(" | ");
    this.container.appendChild(separator);

    this.brushSizeDec = document.createElement("span");
    this.brushSizeDec.style.marginRight = '5px';
    this.brushSizeDec.className = 'button';
    this.brushSizeDec.innerHTML = '-';
    this.container.appendChild(this.brushSizeDec);

    space = document.createTextNode(" ");
    this.container.appendChild(space);

    this.brushSizeInc = document.createElement("span");
    this.brushSizeInc.className = 'button';
    this.brushSizeInc.innerHTML = '+';
    this.container.appendChild(this.brushSizeInc);

    separator = document.createTextNode(" | ");
    this.container.appendChild(separator);

    var brushLabel = document.createTextNode("brush:");
    this.container.appendChild(brushLabel);    
    this.brushSelector = document.createElement("select");
    this.brushSelector.style.marginLeft = "8px";
    this.brushSelector.style.marginRight = "8px";
    for (i = 0; i < BRUSHES.length; i++)
    {
      var option = document.createElement("option");
      option.id = i;
      option.innerHTML = BRUSHES[i].toUpperCase();
      this.brushSelector.appendChild(option);
    }
    this.container.appendChild(this.brushSelector);

    var paperLabel = document.createTextNode(" paper:");
    this.container.appendChild(paperLabel);
    this.paperSelector = document.createElement("select");
    this.paperSelector.style.marginLeft = "8px";
    this.paperSelector.style.marginRight = "8px";
    for (i = 0; i < PAPERS.length; i++)
    {
      var option = document.createElement("option");
      option.id = i;
      option.innerHTML = PAPERS[i].toUpperCase();
      this.paperSelector.appendChild(option);
    }
    this.container.appendChild(this.paperSelector);
    
    separator = document.createTextNode(" | ");
    this.container.appendChild(separator);

    this.clear = document.createElement("span");
    this.clear.className = 'button';
    this.clear.innerHTML = 'Clear';
    this.container.appendChild(this.clear);

    separator = document.createTextNode(" | ");
    this.container.appendChild(separator);

    this.save = document.createElement("span");
    this.save.className = 'button';
    this.save.innerHTML = 'Save';
    this.container.appendChild(this.save);
  },

  setBrushColor: function( color )
  {
    var context = this.brushColor.getContext("2d");
    context.fillStyle = 'rgb(' + color[0] + ', ' + color[1] +', ' + color[2] + ')';
    context.fillRect(0, 0, this.brushColor.width, this.brushColor.height);
    context.fillStyle = 'rgba(0, 0, 0, 0.1)';
    context.fillRect(0, 0, this.brushColor.width, 1);
  },

  setCanvasColor: function( color )
  {
    var context = this.canvasColor.getContext("2d");
    context.fillStyle = 'rgb(' + color[0] + ', ' + color[1] +', ' + color[2] + ')';
    context.fillRect(0, 0, this.canvasColor.width, this.canvasColor.height);
    context.fillStyle = 'rgba(0, 0, 0, 0.1)';
    context.fillRect(0, 0, this.canvasColor.width, 1);
  }
}
