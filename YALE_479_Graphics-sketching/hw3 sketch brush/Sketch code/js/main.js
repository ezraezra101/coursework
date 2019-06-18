// Insert new brushes here. The name of the new brush will be displayed in the
// brush selector and it needs to match the function defined in the javascript
// file of the brush. For example, a function "simple(context)" is defined in
// brushes/simple.js to support "simple" in BRUSHES.
var BRUSHES = ["brush1", "brush2", "brush3", "simple", "sketchy"];

// Insert new papers here. The name of the new paper will be displayed in the
// paper selector and it needs to match the function defined in the javascript
// file of the paper. For example, a function "color(context)" is defined in
// papers/color.js to support "color" in PAPERS.
var PAPERS = [ "paper1", "paper2", "color", "sandy"];
var PAPER_DB = []; // database of the pre-initialize the paper objects defined in PAPERS

// Properties of the current drawing state. These variables are declared as
// global and can be easily accessed in javascript files of the custom brushes.
var BRUSH_SIZE = 1, // the minimum size is 1
    BRUSH_PRESSURE = 1,
    BRUSH_COLOR = [0, 0, 0],
    CANVAS_COLOR = [250, 250, 250];

// Other global variables
var screenWidth = window.innerWidth,
    screenHeight = window.innerHeight,
    brush, // the current active brush
    paper, // the current active canvas paper
    menu,
    canvas, // defined the canvas context of the drawing (post-processed by the paper effect)
    strokeCanvas, // defined the canvas context of the strokes
    bgCanvas, // defined the canvas context of the background
    flattenCanvas, // used to generate (i.e. flatten the strokes and the
                   // background color) the drawing before saved as the a
                   // png image
    context,
    strokeContext,
    bgContext,
    brushColorSelector,
    canvasColorSelector,
    isBrushColorSelectorVisible = false,
    isCanvasColorSelectorVisible = false,
    saveTimeOut,
    hasStorage = true;

init(); // everything start here

// generate the elements inside the main body of index.html
function init()
{
  var agent = navigator.userAgent.toLowerCase();
  if (agent.search("android") > -1 || agent.search("iphone") > -1)
    BRUSH_SIZE = 2;

  if (agent.search("safari") > -1 && agent.search("chrome") == -1)
    hasStorage = false;

  document.body.style.backgroundRepeat = 'no-repeat';
  document.body.style.backgroundPosition = 'center center';

  var container = document.createElement('div');
  document.body.appendChild(container);

  // define the drawing canvas where strokes are going to render on.
  canvas = document.createElement("canvas");
  canvas.id = 'drawing layer';
  canvas.width = screenWidth; // use the full estate of the browser window
  canvas.height = screenHeight;
  canvas.style.position = 'absolute'; // need to postion the canvas at (0,0) to stack it on the background canvas
  canvas.style.left = '0px';
  canvas.style.top = '0px';
  canvas.style.zIndex = '0'; // make it over the background canvas
  canvas.style.cursor = 'crosshair';
  container.appendChild(canvas);
  // get the context of 2D operations of the drawing canvas
  context = canvas.getContext("2d");

  // define the stroke canvas containing the native strokes
  strokeCanvas = document.createElement("canvas");
  strokeCanvas.id = 'stroke layer';
  strokeCanvas.width = screenWidth;
  strokeCanvas.height = screenHeight;
  strokeCanvas.style.position = 'absolute'; // need to postion the canvas at (0,0) to stack it under the drawing canvas
  strokeCanvas.style.left = '0px';
  strokeCanvas.style.top = '0px';
  strokeContext = strokeCanvas.getContext("2d");    
  
  // define the background canvas containing the loaded or generated image.
  bgCanvas = document.createElement("canvas");
  bgCanvas.id = 'background layer';
  bgCanvas.width = screenWidth;
  bgCanvas.height = screenHeight;
  bgCanvas.style.position = 'absolute'; // need to postion the canvas at (0,0) to stack it under the drawing canvas
  bgCanvas.style.left = '0px';
  bgCanvas.style.top = '0px';
  bgCanvas.style.zIndex = '-1'; // make it under the drawing canvas
  container.appendChild(bgCanvas);
  bgContext = bgCanvas.getContext("2d");    

  // create the canvas to hold the flattened rendering of the drawing canvas
  // and the background color.
  flattenCanvas = document.createElement("canvas");
  flattenCanvas.width = screenWidth;
  flattenCanvas.height = screenHeight;

  // pre-initialize the paper object
  for (i = 0; i < PAPERS.length; i++)
  {
    PAPER_DB[i] = eval("new " + PAPERS[i] + "()"); // not a paper works on the background canvas
  }  

  // defined the color picker
  var palette = new Palette();
  brushColorSelector = new ColorSelector(palette);
  brushColorSelector.addEventListener('change', onBrushColorSelectorChange);
  container.appendChild(brushColorSelector.container);
  canvasColorSelector = new ColorSelector(palette);
  canvasColorSelector.addEventListener('change', onCanvasColorSelectorChange);
  container.appendChild(canvasColorSelector.container);

  // define the user menu. Each button will listen both the mouse click (on
  // desktops) and the screen touch (on touchable devices)
  menu = new Menu();
  // define the buttons to toggle the color pickers
  menu.brushColor.addEventListener('click', onMenuBrushColor);
  menu.brushColor.addEventListener('touchend', onMenuBrushColor);
  menu.canvasColor.addEventListener('click', onMenuCanvasColor);
  menu.canvasColor.addEventListener('touchend', onMenuCanvasColor);
  // defined the buttons to increment or decrement the brush size
  menu.brushSizeDec.addEventListener('click', onMenuBrushSizeDec);
  menu.brushSizeDec.addEventListener('touchend', onMenuBrushSizeDec);
  menu.brushSizeInc.addEventListener('click', onMenuBrushSizeInc);
  menu.brushSizeInc.addEventListener('touchend', onMenuBrushSizeInc);
  // define the brush selector (a drop-down list of brushes)
  menu.brushSelector.addEventListener('change', onMenuBrushSelectorChange);
  // define the canvas selector (a drop-down list of canvases)
  menu.paperSelector.addEventListener('change', onMenuPaperSelectorChange);
  // define the button to clear the current drawing
  menu.clear.addEventListener('click', onMenuClear);
  menu.clear.addEventListener('touchend', onMenuClear);
  // define the button to save the current drawing
  menu.save.addEventListener('click', onMenuSave);
  menu.save.addEventListener('touchend', onMenuSave);
  container.appendChild(menu.container);

  if (hasStorage)
  { // restore the drawing states from the previous session    
    if (localStorage.brush_color_red)
    {
      BRUSH_COLOR[0] = localStorage.brush_color_red;
      BRUSH_COLOR[1] = localStorage.brush_color_green;
      BRUSH_COLOR[2] = localStorage.brush_color_blue;
    }

    if (localStorage.background_color_red)
    {
      CANVAS_COLOR[0] = localStorage.background_color_red;
      CANVAS_COLOR[1] = localStorage.background_color_green;
      CANVAS_COLOR[2] = localStorage.background_color_blue;
    }

    if (localStorage.brush_size)
    {
      BRUSH_SIZE = localStorage.brush_size;
    }
    
    if (localStorage.brush_idx)
    {
      menu.brushSelector.selectedIndex = localStorage.brush_idx;        
      brush = eval("new " + BRUSHES[menu.brushSelector.selectedIndex] + "()");
    }
    if (localStorage.paper_idx)
    {
      menu.paperSelector.selectedIndex = localStorage.paper_idx;
      paper = PAPER_DB[menu.paperSelector.selectedIndex];
    }
  }

  // synchronize the selected color to the color pickers
  brushColorSelector.setColor( BRUSH_COLOR );
  canvasColorSelector.setColor( CANVAS_COLOR );

  if (!brush)
  { // if there is no stored brush, select the 1st brush as the default brush
    brush = eval("new " + BRUSHES[0] + "()");
  }

  if (!paper)
  { // if there is no stored paper, select the 1st paper as the default canvas paper
    paper = PAPER_DB[0];
  }

  // listen to the user events
  window.addEventListener('resize', onWindowResize);
  canvas.addEventListener('mousedown', onCanvasMouseDown);
  canvas.addEventListener('touchstart', onCanvasTouchStart);
 
  onWindowResize();
}

function onWindowResize()
{
  screenWidth = window.innerWidth;
  screenHeight = window.innerHeight;

  canvas.width = screenWidth;
  canvas.height = screenHeight;
  context = canvas.getContext("2d");

  strokeCanvas.width = screenWidth;
  strokeCanvas.height = screenHeight;
  strokeContext = strokeCanvas.getContext("2d");
  
  bgCanvas.width = screenWidth;
  bgCanvas.height = screenHeight;
  bgContext = bgCanvas.getContext("2d");

  menu.container.style.width = screenWidth + 'px';
  menu.container.style.left = ((screenWidth - menu.container.offsetWidth) / 2) + 'px';

  paper.apply(true, true);
}

function onBrushColorSelectorChange( event )
{
  BRUSH_COLOR = brushColorSelector.getColor();

  menu.setBrushColor( BRUSH_COLOR );

  if (hasStorage)
  {
    localStorage.brush_color_red = BRUSH_COLOR[0];
    localStorage.brush_color_green = BRUSH_COLOR[1];
    localStorage.brush_color_blue = BRUSH_COLOR[2];
  }
}

function onCanvasColorSelectorChange( event )
{
  CANVAS_COLOR = canvasColorSelector.getColor();

  menu.setCanvasColor( CANVAS_COLOR );

  paper.apply(true, true);

  if (hasStorage)
  {
    localStorage.background_color_red = CANVAS_COLOR[0];
    localStorage.background_color_green = CANVAS_COLOR[1];
    localStorage.background_color_blue = CANVAS_COLOR[2];
  }
}

function onMenuBrushColor()
{
  cleanPopUps();

  brushColorSelector.show();
  brushColorSelector.container.style.left = ((screenWidth - brushColorSelector.container.offsetWidth) / 2) + 'px';
  brushColorSelector.container.style.top = ((screenHeight - brushColorSelector.container.offsetHeight) / 2) + 'px';

  isBrushColorSelectorVisible = true;
}

function onMenuCanvasColor()
{
  cleanPopUps();

  canvasColorSelector.show();
  canvasColorSelector.container.style.left = ((screenWidth - canvasColorSelector.container.offsetWidth) / 2) + 'px';
  canvasColorSelector.container.style.top = ((screenHeight - canvasColorSelector.container.offsetHeight) / 2) + 'px';

  isCanvasColorSelectorVisible = true;
}

function onMenuBrushSizeDec()
{
  cleanPopUps();

  if(BRUSH_SIZE > 1) BRUSH_SIZE --;

  if (hasStorage)
  {
    localStorage.brush_size = BRUSH_SIZE;
  }
}

function onMenuBrushSizeInc()
{
  cleanPopUps();

  BRUSH_SIZE ++;

  if (hasStorage)
  {
    localStorage.brush_size = BRUSH_SIZE;
  }
}

function onMenuBrushSelectorChange()
{
  if (BRUSHES[menu.brushSelector.selectedIndex] == "")
    return;

  brush.destroy();
  brush = eval("new " + BRUSHES[menu.brushSelector.selectedIndex] + "()");

  if (hasStorage)
  {
    localStorage.brush_idx = menu.brushSelector.selectedIndex;
  }
}

function onMenuPaperSelectorChange()
{
  if (PAPERS[menu.paperSelector.selectedIndex] == "")
    return;

  paper = PAPER_DB[menu.paperSelector.selectedIndex];
  paper.apply(true, true);
  
  if (hasStorage)
  {
    localStorage.paper_idx = menu.paperSelector.selectedIndex;
  }
}

function onMenuSave()
{
  flatten();
  window.open(flattenCanvas.toDataURL('image/png'),'mywindow');
}

function onMenuClear()
{
  context.clearRect(0, 0, screenWidth, screenHeight);
  strokeContext.clearRect(0, 0, screenWidth, screenHeight);

  saveToLocalStorage();

  brush.destroy();
  brush = eval("new " + BRUSHES[menu.brushSelector.selectedIndex] + "()");
  
  paper.apply(false, true);
}

function onCanvasMouseDown( event )
{
  clearTimeout(saveTimeOut);
  cleanPopUps();

  BRUSH_PRESSURE = getBrushPressure();

  // pass the starting point to the current brush.
  brush.strokeStart( event.clientX, event.clientY );

  // listen to the mouse movement (to update the strokes) and the mouse up (to
  // finish the stroke).
  window.addEventListener('mousemove', onCanvasMouseMove);
  window.addEventListener('mouseup', onCanvasMouseUp);
}

function onCanvasMouseMove( event )
{
  BRUSH_PRESSURE = getBrushPressure();

  // update the stroke points
  brush.stroke( event.clientX, event.clientY );
  
  paper.apply(true, false);
}

function onCanvasMouseUp()
{
  // finish the stroke.
  brush.strokeEnd();

  // stop listening the mouse movement and mouse up.
  window.removeEventListener('mousemove', onCanvasMouseMove);
  window.removeEventListener('mouseup', onCanvasMouseUp);
  
  if (hasStorage)
  {
    clearTimeout(saveTimeOut);
    saveTimeOut = setTimeout(saveToLocalStorage, 2000, true);
  }
}

function onCanvasTouchStart( event )
{
  cleanPopUps();

  if(event.touches.length == 1)
  { // only handle the 1st touch.
    event.preventDefault();

    // pass the starting point to the current brush.
    brush.strokeStart( event.touches[0].pageX, event.touches[0].pageY );

    // listen to the touch movement (to update the strokes) and the touch up (to
    // finish the stroke).
    window.addEventListener('touchmove', onCanvasTouchMove);
    window.addEventListener('touchend', onCanvasTouchEnd);
  }
}

function onCanvasTouchMove( event )
{
  if(event.touches.length == 1)
  { // only handle the 1st touch.
    event.preventDefault();
    // update the stroke points
    brush.stroke( event.touches[0].pageX, event.touches[0].pageY );
    
    paper.apply(true, false);
  }
}

function onCanvasTouchEnd( event )
{
  if(event.touches.length == 0)
  { // only handle the 1st touch.
    event.preventDefault();

    // finish the stroke.
    brush.strokeEnd();

    // stop listening the mouse movement and mouse up.
    window.removeEventListener('touchmove', onCanvasTouchMove);
    window.removeEventListener('touchend', onCanvasTouchEnd);
  }
}

function saveToLocalStorage()
{
}

function flatten()
{
  var context = flattenCanvas.getContext("2d");

  // note the drawing order here
  context.drawImage(bgCanvas, 0, 0);
  context.drawImage(canvas, 0, 0);
}

function cleanPopUps()
{
  if (isBrushColorSelectorVisible)
  {
    brushColorSelector.hide();
    isBrushColorSelectorVisible = false;
  }

  if (isCanvasColorSelectorVisible)
  {
    canvasColorSelector.hide();
    isCanvasColorSelectorVisible = false;
  }
}

function getBrushPressure()
{
  // you can extend this function if there is a pressure-sensitive stylus
  
  // Again, this snippet is inspired by https://github.com/evhan55/ploma
  plugin = document.getElementById('wtPlugin');
  if(!plugin) {
    return 1;
  }
  penAPI = plugin.penAPI;
  return penAPI.pressure ? penAPI.pressure : 1;
  //return hasStylus ? stylus.pressure : 1;
  //return 1;
}
