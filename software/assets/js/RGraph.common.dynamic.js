
if(typeof(RGraph)=='undefined')RGraph={isRGraph:true,type:'common'};RGraph.InstallWindowMousedownListener=function(obj)
{window.onmousedown=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(typeof(window.onmousedown_rgraph)=='function'){window.onmousedown_rgraph(e);}
if(RGraph.HideTooltip&&RGraph.Registry.Get('chart.tooltip')){RGraph.Clear(RGraph.Registry.Get('chart.tooltip').__canvas__);RGraph.Redraw();RGraph.HideTooltip();}}}
RGraph.InstallWindowMouseupListener=function(obj)
{window.onmouseup=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(RGraph.Annotating_window_onmouseup){RGraph.Annotating_window_onmouseup(e);return;}
if(typeof(window.onmouseup_rgraph)=='function'){window.onmouseup_rgraph(e);}
if(RGraph.Registry.Get('chart.adjusting')||RGraph.Registry.Get('chart.adjusting.gantt'))
{RGraph.FireCustomEvent(RGraph.Registry.Get('chart.adjusting'),'onadjustend');RGraph.Registry.Set('chart.adjusting',null);RGraph.Registry.Set('chart.adjusting.shape',null);RGraph.Registry.Set('chart.adjusting.gantt',null);}
var tags=document.getElementsByTagName('canvas');for(var i=0;i<tags.length;++i){if(tags[i].__object__&&tags[i].__object__.isRGraph){if(!tags[i].__object__.Get('chart.annotatable')){if(!tags[i].__rgraph_trace_cover__&&!noredraw){RGraph.Clear(tags[i]);}else{var noredraw=true;}}}}
if(!noredraw){RGraph.Redraw();}}}
RGraph.InstallCanvasMouseupListener=function(obj)
{obj.canvas.onmouseup=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(typeof(e.target.onmouseup_rgraph)=='function'){e.target.onmouseup_rgraph(e);}
var objects=RGraph.ObjectRegistry.getObjectsByXY(e);if(objects){for(var i=0;i<objects.length;++i){var obj=objects[i];var id=objects[i].id;if(!RGraph.is_null(obj)&&RGraph.Tooltip){var shape=obj.getShape(e);if(shape&&shape['tooltip']){var text=shape['tooltip'];if(text){var type=shape['object'].type;if(type=='line'||type=='rscatter'||(type=='scatter'&&!obj.Get('chart.boxplot'))||type=='radar'){var canvasXY=RGraph.getCanvasXY(obj.canvas);var x=canvasXY[0]+shape['x'];var y=canvasXY[1]+shape['y'];}else{var x=e.pageX;var y=e.pageY;}
if(obj.Get('chart.tooltips.coords.page')){var x=e.pageX;var y=e.pageY;}
RGraph.Clear(obj.canvas);RGraph.Redraw();obj.Highlight(shape);RGraph.Registry.Set('chart.tooltip.shape',shape);RGraph.Tooltip(obj,text,x,y,shape['index'],e);if(RGraph.Registry.Get('chart.tooltip')){RGraph.Registry.Get('chart.tooltip').__shape__=shape;RGraph.EvaluateCursor(e);}
e.cancelBubble=true;e.stopPropagation();return false;}}}}
if(typeof(InteractiveKey_line_mouseup)=='function')InteractiveKey_line_mouseup(e);if(typeof(InteractiveKey_pie_mouseup)=='function')InteractiveKey_pie_mouseup(e);}}}
RGraph.InstallCanvasMousemoveListener=function(obj)
{obj.canvas.onmousemove=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(typeof(e.target.onmousemove_rgraph)=='function'){e.target.onmousemove_rgraph(e);}
var objects=RGraph.ObjectRegistry.getObjectsByXY(e);if(objects&&objects.length){for(var i=0;i<objects.length;++i){var obj=objects[i];var id=obj.id;if(!obj.getShape){continue;}
var shape=obj.getShape(e);var func=obj.Get('chart.events.mousemove');if(shape&&typeof(func)=='function'){if(obj.Get('chart.events.mousemove.revertto')==null){obj.Set('chart.events.mousemove.revertto',e.target.style.cursor);}
func(e,shape);}else if(typeof(obj.Get('chart.events.mousemove.revertto'))=='string'){RGraph.cursor.push('default');obj.Set('chart.events.mousemove.revertto',null);}
if(shape&&(obj.Get('chart.tooltips')&&obj.Get('chart.tooltips')[shape['index']]||shape['tooltip'])&&obj.Get('chart.tooltips.event')=='onmousemove'&&(RGraph.is_null(RGraph.Registry.Get('chart.tooltip'))||RGraph.Registry.Get('chart.tooltip').__index__!=shape['index']||(typeof(shape['dataset'])=='number'&&shape['dataset']!=RGraph.Registry.Get('chart.tooltip').__shape__['dataset'])||obj.uid!=RGraph.Registry.Get('chart.tooltip').__object__.uid)){RGraph.Clear(obj.canvas);RGraph.Redraw();obj.canvas.onmouseup(e);return;}
if(obj&&obj.Get('chart.adjustable')){switch(obj.type){case'bar':obj.Adjusting_mousemove(e);break;case'gauge':obj.Adjusting_mousemove(e);break;case'gantt':obj.Adjusting_mousemove(e);break;case'hprogress':obj.Adjusting_mousemove(e);break;case'line':obj.Adjusting_mousemove(e);break;case'thermometer':obj.Adjusting_mousemove(e);break;case'vprogress':obj.Adjusting_mousemove(e);break;default:obj.Adjusting_mousemove(e);break;}}}}
if(e.target&&e.target.__object__.Get('chart.crosshairs')){RGraph.DrawCrosshairs(e,e.target.__object__);}
if(typeof(InteractiveKey_line_mousemove)=='function')InteractiveKey_line_mousemove(e);if(typeof(InteractiveKey_pie_mousemove)=='function')InteractiveKey_pie_mousemove(e);if(e.target.__object__.Get('chart.annotatable')&&RGraph.Annotating_canvas_onmousemove){RGraph.Annotating_canvas_onmousemove(e);}
RGraph.EvaluateCursor(e);}}
RGraph.InstallCanvasMousedownListener=function(obj)
{obj.canvas.onmousedown=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(typeof(e.target.onmousedown_rgraph)=='function'){e.target.onmousedown_rgraph(e);}
if(e.target.__object__.Get('chart.annotatable')&&RGraph.Annotating_canvas_onmousedown){RGraph.Annotating_canvas_onmousedown(e);return;}
var obj=RGraph.ObjectRegistry.getObjectByXY(e);if(obj){var id=obj.id;if(obj&&obj.isRGraph&&obj.Get('chart.adjustable')){var obj=RGraph.ObjectRegistry.getObjectByXY(e);if(obj&&obj.isRGraph){switch(obj.type){case'bar':var shape=obj.getShapeByX(e);break;case'gantt':var shape=obj.getShape(e);if(shape){var mouseXY=RGraph.getMouseXY(e);RGraph.Registry.Set('chart.adjusting.gantt',{'index':shape['index'],'object':obj,'mousex':mouseXY[0],'mousey':mouseXY[1],'event_start':obj.Get('chart.events')[shape['index']][0],'event_duration':obj.Get('chart.events')[shape['index']][1],'mode':(mouseXY[0]>(shape['x']+shape['width']-5)?'resize':'move'),'shape':shape});}
break;case'line':var shape=obj.getShape(e);break;default:var shape=null;}
RGraph.Registry.Set('chart.adjusting.shape',shape);RGraph.FireCustomEvent(obj,'onadjustbegin');RGraph.Registry.Set('chart.adjusting',obj);RGraph.Clear(obj.canvas);RGraph.Redraw();obj.canvas.onmousemove(e);}}
RGraph.Clear(obj.canvas);RGraph.Redraw();}}}
RGraph.InstallCanvasClickListener=function(obj)
{obj.canvas.onclick=function(e)
{if(navigator.userAgent.indexOf('Firefox')>=0)window.event=e;e=RGraph.FixEventObject(e);if(typeof(e.target.onclick_rgraph)=='function'){e.target.onclick_rgraph(e);}
var objects=RGraph.ObjectRegistry.getObjectsByXY(e);for(var i=0;i<objects.length;++i){var obj=objects[i];var id=obj.id;var shape=obj.getShape(e);var func=obj.Get('chart.events.click');if(shape&&typeof(func)=='function'){func(e,shape);return;}}}}
RGraph.EvaluateCursor=function(e)
{var mouseXY=RGraph.getMouseXY(e);var mouseX=mouseXY[0];var mouseY=mouseXY[1];var objects=RGraph.ObjectRegistry.getObjectsByXY(e);for(var i=0;i<objects.length;++i){var obj=objects[i]
var id=obj.id;if(!RGraph.is_null(obj)){if(obj.getShape&&obj.getShape(e)){var shape=obj.getShape(e);if(obj.Get('chart.tooltips')){var text=RGraph.parseTooltipText(obj.Get('chart.tooltips'),shape['index']);if(!text&&shape['object'].type=='scatter'&&shape['index_adjusted']){text=RGraph.parseTooltipText(obj.Get('chart.tooltips'),shape['index_adjusted']);}
if(text){var pointer=true;}}}
if(!RGraph.is_null(obj)&&obj.Get('chart.key.interactive')){for(var j=0;j<obj.coords.key.length;++j){if(mouseX>obj.coords.key[j][0]&&mouseX<(obj.coords.key[j][0]+obj.coords.key[j][2])&&mouseY>obj.coords.key[j][1]&&mouseY<(obj.coords.key[j][1]+obj.coords.key[j][3])){var pointer=true;}}}}
if(!RGraph.is_null(shape)&&!RGraph.is_null(obj)&&!RGraph.is_null(obj.Get('chart.events.mousemove'))&&typeof(obj.Get('chart.events.mousemove'))=='function'){var str=(obj.Get('chart.events.mousemove')).toString();if(str.match(/pointer/)&&str.match(/cursor/)&&str.match(/style/)){var pointer=true;}}}
var objects=RGraph.ObjectRegistry.objects.byCanvasID;for(var i=0;i<objects.length;++i){if(objects[i]&&objects[i][1].Get('chart.resizable')){var resizable=true;}}
if(resizable&&mouseX>(e.target.width-32)&&mouseY>(e.target.height-16)){pointer=true;}
if(pointer){e.target.style.cursor='pointer';}else if(e.target.style.cursor=='pointer'){e.target.style.cursor='default';}else{e.target.style.cursor=null;}
if(resizable&&mouseX>=(e.target.width-15)&&mouseY>=(e.target.height-15)){e.target.style.cursor='move';}
if(typeof(mouse_over_key)=='boolean'&&mouse_over_key){e.target.style.cursor='pointer';}
if(obj&&obj.type=='gantt'&&obj.Get('chart.adjustable')){if(obj.getShape&&obj.getShape(e)){e.target.style.cursor='ew-resize';}else{e.target.style.cursor='default';}}
if(obj&&obj.type=='line'&&obj.Get('chart.adjustable')){if(obj.getShape&&obj.getShape(e)){e.target.style.cursor='ns-resize';}else{e.target.style.cursor='default';}}
if(e.target.__object__.Get('chart.annotatable')){e.target.style.cursor='crosshair';}}
RGraph.parseTooltipText=function(tooltips,idx)
{if(!tooltips){return null;}
if(typeof(tooltips)=='function'){var text=tooltips(idx);}else if(typeof(tooltips)=='string'){var text=tooltips;}else if(typeof(tooltips)=='object'&&typeof(tooltips)[idx]=='function'){var text=tooltips[idx](idx);}else if(typeof(tooltips)[idx]=='string'&&tooltips[idx]){var text=tooltips[idx];}else{var text='';}
if(text=='undefined'){text='';}else if(text=='null'){text='';}
return RGraph.getTooltipTextFromDIV?RGraph.getTooltipTextFromDIV(text):text;}
RGraph.DrawCrosshairs=function(e,obj)
{var e=RGraph.FixEventObject(e);var width=obj.canvas.width;var height=obj.canvas.height;var mouseXY=RGraph.getMouseXY(e);var x=mouseXY[0];var y=mouseXY[1];RGraph.RedrawCanvas(obj.canvas);if(x>=obj.gutterLeft&&y>=obj.gutterTop&&x<=(width-obj.gutterRight)&&y<=(height-obj.gutterBottom)){var linewidth=obj.Get('chart.crosshairs.linewidth')?obj.Get('chart.crosshairs.linewidth'):1;obj.context.lineWidth=linewidth?linewidth:1;obj.context.beginPath();obj.context.strokeStyle=obj.Get('chart.crosshairs.color');if(obj.Get('chart.crosshairs.vline')){obj.context.moveTo(AA(this,x),AA(this,obj.gutterTop));obj.context.lineTo(AA(this,x),AA(this,height-obj.gutterBottom));}
if(obj.Get('chart.crosshairs.hline')){obj.context.moveTo(AA(this,obj.gutterLeft),AA(this,y));obj.context.lineTo(AA(this,width-obj.gutterRight),AA(this,y));}
obj.context.stroke();if(obj.Get('chart.crosshairs.coords')){if(obj.type=='scatter'){var xCoord=(((x-obj.Get('chart.gutter.left'))/(obj.canvas.width-obj.gutterLeft-obj.gutterRight))*(obj.Get('chart.xmax')-obj.Get('chart.xmin')))+obj.Get('chart.xmin');xCoord=xCoord.toFixed(obj.Get('chart.scale.decimals'));var yCoord=obj.max-(((y-obj.Get('chart.gutter.top'))/(obj.canvas.height-obj.gutterTop-obj.gutterBottom))*obj.max);if(obj.type=='scatter'&&obj.Get('chart.xaxispos')=='center'){yCoord=(yCoord-(obj.max/2))*2;}
yCoord=yCoord.toFixed(obj.Get('chart.scale.decimals'));var div=RGraph.Registry.Get('chart.coordinates.coords.div');var mouseXY=RGraph.getMouseXY(e);var canvasXY=RGraph.getCanvasXY(obj.canvas);if(!div){var div=document.createElement('DIV');div.__object__=obj;div.style.position='absolute';div.style.backgroundColor='white';div.style.border='1px solid black';div.style.fontFamily='Arial, Verdana, sans-serif';div.style.fontSize='10pt'
div.style.padding='2px';div.style.opacity=1;div.style.WebkitBorderRadius='3px';div.style.borderRadius='3px';div.style.MozBorderRadius='3px';document.body.appendChild(div);RGraph.Registry.Set('chart.coordinates.coords.div',div);}
div.style.opacity=1;div.style.display='inline';if(!obj.Get('chart.crosshairs.coords.fixed')){div.style.left=Math.max(2,(e.pageX-div.offsetWidth-3))+'px';div.style.top=Math.max(2,(e.pageY-div.offsetHeight-3))+'px';}else{div.style.left=canvasXY[0]+obj.gutterLeft+3+'px';div.style.top=canvasXY[1]+obj.gutterTop+3+'px';}
div.innerHTML='<span style="color: #666">'+obj.Get('chart.crosshairs.coords.labels.x')+':</span> '+xCoord+'<br><span style="color: #666">'+obj.Get('chart.crosshairs.coords.labels.y')+':</span> '+yCoord;obj.canvas.addEventListener('mouseout',RGraph.HideCrosshairCoords,false);obj.canvas.__crosshairs_labels__=div;obj.canvas.__crosshairs_x__=xCoord;obj.canvas.__crosshairs_y__=yCoord;}else{alert('[RGRAPH] Showing crosshair coordinates is only supported on the Scatter chart');}}
RGraph.FireCustomEvent(obj,'oncrosshairs');}else{RGraph.HideCrosshairCoords();}}