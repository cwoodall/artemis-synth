
if(typeof(RGraph)=='undefined')RGraph={isRGraph:true,type:'common'};RGraph.Registry={};RGraph.Registry.store=[];RGraph.Registry.store['chart.event.handlers']=[];RGraph.Registry.store['__rgraph_event_listeners__']=[];RGraph.background={};RGraph.objects=[];RGraph.Resizing={};RGraph.events=[];RGraph.cursor=[];RGraph.ObjectRegistry={};RGraph.ObjectRegistry.objects={};RGraph.ObjectRegistry.objects.byUID=[];RGraph.ObjectRegistry.objects.byCanvasID=[];HALFPI=(Math.PI/2);PI=Math.PI;TWOPI=PI*2;RGraph.getScale=function(max,obj)
{if(max==0){return['0.2','0.4','0.6','0.8','1.0'];}
var original_max=max;if(max<=1){if(max>0.5){return[0.2,0.4,0.6,0.8,Number(1).toFixed(1)];}else if(max>=0.1){return obj.Get('chart.scale.round')?[0.2,0.4,0.6,0.8,1]:[0.1,0.2,0.3,0.4,0.5];}else{var tmp=max;var exp=0;while(tmp<1.01){exp+=1;tmp*=10;}
var ret=['2e-'+exp,'4e-'+exp,'6e-'+exp,'8e-'+exp,'10e-'+exp];if(max<=('5e-'+exp)){ret=['1e-'+exp,'2e-'+exp,'3e-'+exp,'4e-'+exp,'5e-'+exp];}
return ret;}}
if(String(max).indexOf('.')>0){max=String(max).replace(/\.\d+$/,'');}
var interval=Math.pow(10,Number(String(Number(max)).length-1));var topValue=interval;while(topValue<max){topValue+=(interval/2);}
if(Number(original_max)>Number(topValue)){topValue+=(interval/2);}
if(max<10){topValue=(Number(original_max)<=5?5:10);}
if(obj&&typeof(obj.Get('chart.scale.round'))=='boolean'&&obj.Get('chart.scale.round')){topValue=10*interval;}
return[topValue*0.2,topValue*0.4,topValue*0.6,topValue*0.8,topValue];}
RGraph.array_max=function(arr)
{var max=null;if(typeof(arr)=='number'){return arr;}
for(var i=0;i<arr.length;++i){if(typeof(arr[i])=='number'){var val=arguments[1]?Math.abs(arr[i]):arr[i];if(typeof(max)=='number'){max=Math.max(max,val);}else{max=val;}}}
return max;}
RGraph.array_pad=function(arr,len)
{if(arr.length<len){var val=arguments[2]?arguments[2]:null;for(var i=arr.length;i<len;++i){arr[i]=val;}}
return arr;}
RGraph.array_sum=function(arr)
{if(typeof(arr)=='number'){return arr;}
var i,sum;var len=arr.length;for(i=0,sum=0;i<len;sum+=arr[i++]);return sum;}
RGraph.array_linearize=function()
{var arr=[];for(var i=0;i<arguments.length;++i){if(typeof(arguments[i])=='object'&&arguments[i]){for(var j=0;j<arguments[i].length;++j){var sub=RGraph.array_linearize(arguments[i][j]);for(var k=0;k<sub.length;++k){arr.push(sub[k]);}}}else{arr.push(arguments[i]);}}
return arr;}
RGraph.Text=function(context,font,size,x,y,text)
{if((typeof(text)!='string'&&typeof(text)!='number')||text=='undefined'){return;}
if(typeof(text)=='string'){if(text.indexOf('\r\n')>0&&arguments[6]=='center'){var line_count=text.split('\r\n').length;y=y-((line_count-1.5)*size);}else if(text.indexOf('\r\n')>0&&arguments[6]=='bottom'){var line_count=text.split('\r\n').length;y=y-((line_count-0.5)*size);}}
if(typeof(text)=='string'&&text.match(/\r\n/)){var arr=text.split('\r\n');text=arr[0];arr=RGraph.array_shift(arr);var nextline=arr.join('\r\n')
RGraph.Text(context,font,size,arguments[9]==-90?(x+(size*1.5)):x,y+(size*1.5),nextline,arguments[6]?arguments[6]:null,arguments[7],arguments[8],arguments[9],arguments[10],arguments[11],arguments[12]);}
if(RGraph.isOld()){y+=2;}
context.font=(arguments[11]?'Bold ':'')+size+'pt '+font;var i;var origX=x;var origY=y;var originalFillStyle=context.fillStyle;var originalLineWidth=context.lineWidth;if(typeof(arguments[6])==null)arguments[6]='bottom';if(typeof(arguments[7])==null)arguments[7]='left';if(typeof(arguments[8])==null)arguments[8]=null;if(typeof(arguments[9])==null)arguments[9]=0;if(typeof(arguments[12])==null)arguments[12]=true;if(navigator.userAgent.indexOf('Opera')!=-1){context.canvas.__rgraph_valign__=arguments[6];context.canvas.__rgraph_halign__=arguments[7];}
context.save();context.canvas.__rgraph_originalx__=x;context.canvas.__rgraph_originaly__=y;context.translate(x,y);x=0;y=0;if(arguments[9]){context.rotate(arguments[9]/57.3);}
if(arguments[6]){var vAlign=arguments[6];if(vAlign=='center'){context.translate(0,size/2);}else if(vAlign=='top'){context.translate(0,size);}}
if(arguments[7]){var hAlign=arguments[7];var width=context.measureText(text).width;if(hAlign){if(hAlign=='center'){context.translate(-1*(width/2),0)}else if(hAlign=='right'){context.translate(-1*width,0)}}}
context.fillStyle=originalFillStyle;context.save();context.fillText(text,0,0);context.lineWidth=1;if(arguments[8]){var width=context.measureText(text).width;var ieOffset=RGraph.isIE8()?2:0;context.translate(x,y);context.strokeRect(AA(context.canvas.__object__,-3),AA(context.canvas.__object__,0-3-size-ieOffset),width+6,0+size+6);if(arguments[10]){var offset=3;var ieOffset=RGraph.isIE8()?2:0;var width=context.measureText(text).width
context.fillStyle=arguments[10];context.fillRect(AA(context.canvas.__object__,x-offset),AA(context.canvas.__object__,y-size-offset-ieOffset),width+(2*offset),size+(2*offset));}
context.fillStyle=originalFillStyle;if(arguments[6]='center'){context.ttextBaseline='middle';}
context.fillText(text,0,0);if(arguments[12]){context.fillRect(arguments[7]=='left'?0:(arguments[7]=='center'?width/2:width)-2,arguments[6]=='bottom'?0:(arguments[6]=='center'?(0-size)/2:0-size)-2,4,4);}}
context.restore();context.lineWidth=originalLineWidth;context.restore();}
RGraph.Clear=function(canvas)
{if(!canvas){return;}
RGraph.FireCustomEvent(canvas.__object__,'onbeforeclear');var context=canvas.getContext('2d');var color=arguments[1];if(RGraph.isIE8()&&!color){color='white';}
if(!color||(color&&color=='transparent')){context.clearRect(0,0,canvas.width,canvas.height);context.globalCompositeOperation='source-over';}else{context.fillStyle=color;context=canvas.getContext('2d');context.beginPath();if(RGraph.isIE8()){context.fillRect(0,0,canvas.width,canvas.height);}else{context.fillRect(-10,-10,canvas.width+20,canvas.height+20);}
context.fill();}
if(RGraph.ClearAnnotations){}
if(RGraph.Registry.Get('chart.background.image.'+canvas.id)){var img=RGraph.Registry.Get('chart.background.image.'+canvas.id);img.style.position='absolute';img.style.left='-10000px';img.style.top='-10000px';}
if(RGraph.Registry.Get('chart.tooltip')){RGraph.HideTooltip();}
canvas.style.cursor='default';RGraph.FireCustomEvent(canvas.__object__,'onclear');}
RGraph.DrawTitle=function(obj,text,gutterTop)
{var canvas=obj.canvas;var context=obj.context;var gutterLeft=obj.Get('chart.gutter.left');var gutterRight=obj.Get('chart.gutter.right');var gutterBottom=obj.Get('chart.gutter.bottom');var size=arguments[4]?arguments[4]:12;var bold=obj.Get('chart.title.bold');var centerx=(arguments[3]?arguments[3]:((obj.canvas.width-gutterLeft-gutterRight)/2)+gutterLeft);var keypos=obj.Get('chart.key.position');var vpos=obj.Get('chart.title.vpos');var hpos=obj.Get('chart.title.hpos');var bgcolor=obj.Get('chart.title.background');if(obj.type=='bar'&&obj.Get('chart.variant')=='3d'){keypos='gutter';}
context.beginPath();context.fillStyle=obj.Get('chart.text.color')?obj.Get('chart.text.color'):'black';if(keypos&&keypos!='gutter'){var vCenter='center';}else if(!keypos){var vCenter='center';}else{var vCenter='bottom';}
if(typeof(obj.Get('chart.title.vpos'))=='number'){vpos=obj.Get('chart.title.vpos')*gutterTop;if(obj.Get('chart.xaxispos')=='top'){vpos=obj.Get('chart.title.vpos')*gutterBottom+gutterTop+(obj.canvas.height-gutterTop-gutterBottom);}}else{vpos=gutterTop-size-5;if(obj.Get('chart.xaxispos')=='top'){vpos=obj.canvas.height-gutterBottom+size+5;}}
if(typeof(hpos)=='number'){centerx=hpos*canvas.width;}
if(typeof(obj.Get('chart.title.color')!=null)){var oldColor=context.fillStyle
var newColor=obj.Get('chart.title.color')
context.fillStyle=newColor?newColor:'black';}
var font=obj.Get('chart.text.font');if(typeof(obj.Get('chart.title.font'))=='string'){font=obj.Get('chart.title.font');}
RGraph.Text(context,font,size,centerx,vpos,text,vCenter,'center',bgcolor!=null,null,bgcolor,bold);context.fillStyle=oldColor;}
RGraph.getMouseXY=function(e)
{var el=((RGraph.isIE8()||RGraph.isIE7())?event.srcElement:e.target);var x;var y;var paddingLeft=el.style.paddingLeft?parseInt(el.style.paddingLeft):0;var paddingTop=el.style.paddingTop?parseInt(el.style.paddingTop):0;var borderLeft=el.style.borderLeftWidth?parseInt(el.style.borderLeftWidth):0;var borderTop=el.style.borderTopWidth?parseInt(el.style.borderTopWidth):0;if(RGraph.isIE8())e=event;if(typeof(e.offsetX)=='number'&&typeof(e.offsetY)=='number'){x=e.offsetX;y=e.offsetY;}else{x=0;y=0;while(el!=document.body&&el){x+=el.offsetLeft;y+=el.offsetTop;el=el.offsetParent;}
x=e.pageX-x;y=e.pageY-y;}
return[x,y];}
RGraph.getCanvasXY=function(canvas)
{var x=0;var y=0;var obj=canvas;do{x+=obj.offsetLeft;y+=obj.offsetTop;obj=obj.offsetParent;}while(obj&&obj.tagName.toLowerCase()!='body');var paddingLeft=canvas.style.paddingLeft?parseInt(canvas.style.paddingLeft):0;var paddingTop=canvas.style.paddingTop?parseInt(canvas.style.paddingTop):0;var borderLeft=canvas.style.borderLeftWidth?parseInt(canvas.style.borderLeftWidth):0;var borderTop=canvas.style.borderTopWidth?parseInt(canvas.style.borderTopWidth):0;return[x+paddingLeft+borderLeft,y+paddingTop+borderTop];}
RGraph.Register=function(obj)
{if(!obj.Get('chart.noregister')){RGraph.ObjectRegistry.Add(obj);obj.Set('chart.noregister',true);}}
RGraph.Redraw=function()
{var objectRegistry=RGraph.ObjectRegistry.objects.byCanvasID;var tags=document.getElementsByTagName('canvas');for(var i=0;i<tags.length;++i){if(tags[i].__object__&&tags[i].__object__.isRGraph){RGraph.Clear(tags[i],arguments[0]?arguments[0]:null);}}
for(var i=0;i<objectRegistry.length;++i){if(objectRegistry[i]){var id=objectRegistry[i][0];objectRegistry[i][1].Draw();}}}
RGraph.RedrawCanvas=function(canvas)
{var objects=RGraph.ObjectRegistry.getObjectsByCanvasID(canvas.id);if(!arguments[1]||(typeof(arguments[1])=='boolean'&&!arguments[1]==false)){RGraph.Clear(canvas);}
for(var i=0;i<objects.length;++i){if(objects[i]){if(objects[i]&&objects[i].isRGraph){objects[i].Draw();}}}}
RGraph.background.Draw=function(obj)
{var canvas=obj.canvas;var context=obj.context;var height=0;var gutterLeft=obj.Get('chart.gutter.left');var gutterRight=obj.Get('chart.gutter.right');var gutterTop=obj.Get('chart.gutter.top');var gutterBottom=obj.Get('chart.gutter.bottom');var variant=obj.Get('chart.variant');context.fillStyle=obj.Get('chart.text.color');if(variant=='3d'){context.save();context.translate(10,-5);}
if(typeof(obj.Get('chart.title.xaxis'))=='string'&&obj.Get('chart.title.xaxis').length){var size=obj.Get('chart.text.size')+2;var font=obj.Get('chart.text.font');var bold=obj.Get('chart.title.xaxis.bold');if(typeof(obj.Get('chart.title.xaxis.size'))=='number'){size=obj.Get('chart.title.xaxis.size');}
if(typeof(obj.Get('chart.title.xaxis.font'))=='string'){font=obj.Get('chart.title.xaxis.font');}
var hpos=((obj.canvas.width-obj.gutterLeft-obj.gutterRight)/2)+obj.gutterLeft;var vpos=obj.canvas.height-obj.Get('chart.gutter.bottom')+25;if(typeof(obj.Get('chart.title.xaxis.pos'))=='number'){vpos=obj.canvas.height-(gutterBottom*obj.Get('chart.title.xaxis.pos'));}
context.beginPath();RGraph.Text(context,font,size,hpos,vpos,obj.Get('chart.title.xaxis'),'center','center',false,false,false,bold);context.fill();}
if(typeof(obj.Get('chart.title.yaxis'))=='string'&&obj.Get('chart.title.yaxis').length){var size=obj.Get('chart.text.size')+2;var font=obj.Get('chart.text.font');var angle=270;var bold=obj.Get('chart.title.yaxis.bold');if(typeof(obj.Get('chart.title.yaxis.pos'))=='number'){var yaxis_title_pos=obj.Get('chart.title.yaxis.pos')*obj.Get('chart.gutter.left');}else{var yaxis_title_pos=((obj.Get('chart.gutter.left')-25)/obj.Get('chart.gutter.left'))*obj.Get('chart.gutter.left');}
if(typeof(obj.Get('chart.title.yaxis.size'))=='number'){size=obj.Get('chart.title.yaxis.size');}
if(typeof(obj.Get('chart.title.yaxis.font'))=='string'){font=obj.Get('chart.title.yaxis.font');}
if(obj.Get('chart.title.yaxis.align')=='right'||obj.Get('chart.title.yaxis.position')=='right'){angle=90;yaxis_title_pos=obj.Get('chart.title.yaxis.pos')?obj.Get('chart.title.yaxis.pos')*obj.Get('chart.gutter.right'):obj.canvas.width-obj.Get('chart.gutter.right')+obj.Get('chart.text.size')+5;}else{yaxis_title_pos=yaxis_title_pos;}
context.beginPath();RGraph.Text(context,font,size,yaxis_title_pos,((obj.canvas.height-obj.gutterTop-obj.gutterBottom)/2)+obj.gutterTop,obj.Get('chart.title.yaxis'),'center','center',false,angle,false,bold);context.fill();}
obj.context.beginPath();context.fillStyle=obj.Get('chart.background.barcolor1');height=(obj.canvas.height-gutterBottom);for(var i=gutterTop;i<height;i+=80){obj.context.fillRect(gutterLeft,i,RGraph.GetWidth(obj)-gutterLeft-gutterRight,Math.min(40,RGraph.GetHeight(obj)-gutterBottom-i));}
context.fillStyle=obj.Get('chart.background.barcolor2');height=(RGraph.GetHeight(obj)-gutterBottom);for(var i=(40+gutterTop);i<height;i+=80){obj.context.fillRect(gutterLeft,i,RGraph.GetWidth(obj)-gutterLeft-gutterRight,i+40>(RGraph.GetHeight(obj)-gutterBottom)?RGraph.GetHeight(obj)-(gutterBottom+i):40);}
context.stroke();if(obj.Get('chart.background.grid')){if(obj.Get('chart.background.grid.autofit')){if(obj.Get('chart.background.grid.autofit.align')){obj.Set('chart.background.grid.autofit.numhlines',obj.Get('chart.ylabels.count'));if(obj.type=='line'){if(obj.Get('chart.labels')&&obj.Get('chart.labels').length){obj.Set('chart.background.grid.autofit.numvlines',obj.Get('chart.labels').length-1);}else{obj.Set('chart.background.grid.autofit.numvlines',obj.data[0].length-1);}}else if(obj.type=='bar'&&obj.Get('chart.labels')&&obj.Get('chart.labels').length){obj.Set('chart.background.grid.autofit.numvlines',obj.Get('chart.labels').length);}}
var vsize=((obj.canvas.width-gutterLeft-gutterRight))/obj.Get('chart.background.grid.autofit.numvlines');var hsize=(obj.canvas.height-gutterTop-gutterBottom)/obj.Get('chart.background.grid.autofit.numhlines');obj.Set('chart.background.grid.vsize',vsize);obj.Set('chart.background.grid.hsize',hsize);}
context.beginPath();context.lineWidth=obj.Get('chart.background.grid.width')?obj.Get('chart.background.grid.width'):1;context.strokeStyle=obj.Get('chart.background.grid.color');if(obj.Get('chart.background.grid.hlines')){height=(RGraph.GetHeight(obj)-gutterBottom)
for(y=gutterTop;y<height;y+=obj.Get('chart.background.grid.hsize')){context.moveTo(gutterLeft,AA(this,y));context.lineTo(RGraph.GetWidth(obj)-gutterRight,AA(this,y));}}
if(obj.Get('chart.background.grid.vlines')){var width=(obj.canvas.width-gutterRight)
for(x=gutterLeft;x<=width;x+=obj.Get('chart.background.grid.vsize')){context.moveTo(AA(this,x),gutterTop);context.lineTo(AA(this,x),obj.canvas.height-gutterBottom);}}
if(obj.Get('chart.background.grid.border')){context.strokeStyle=obj.Get('chart.background.grid.color');context.strokeRect(AA(this,gutterLeft),AA(this,gutterTop),RGraph.GetWidth(obj)-gutterLeft-gutterRight,RGraph.GetHeight(obj)-gutterTop-gutterBottom);}}
context.stroke();if(variant=='3d'){context.restore();}
if(typeof(obj.Get('chart.title'))=='string'){if(obj.type=='gantt'){gutterTop-=10;}
RGraph.DrawTitle(obj,obj.Get('chart.title'),gutterTop,null,obj.Get('chart.title.size')?obj.Get('chart.title.size'):obj.Get('chart.text.size')+2);}
context.stroke();}
RGraph.GetDays=function(obj)
{var year=obj.getFullYear();var days=obj.getDate();var month=obj.getMonth();if(month==0)return days;if(month>=1)days+=31;if(month>=2)days+=28;if(year>=2008&&year%4==0)days+=1;if(month>=3)days+=31;if(month>=4)days+=30;if(month>=5)days+=31;if(month>=6)days+=30;if(month>=7)days+=31;if(month>=8)days+=31;if(month>=9)days+=30;if(month>=10)days+=31;if(month>=11)days+=30;return days;}
RGraph.array_clone=function(obj)
{if(obj==null||typeof(obj)!='object'){return obj;}
var temp=[];for(var i=0;i<obj.length;++i){if(typeof(obj[i])=='number'){temp[i]=(function(arg){return Number(arg);})(obj[i]);}else if(typeof(obj[i])=='string'){temp[i]=(function(arg){return String(arg);})(obj[i]);}else if(typeof(obj[i])=='function'){temp[i]=obj[i];}else{temp[i]=RGraph.array_clone(obj[i]);}}
return temp;}
RGraph.number_format=function(obj,num)
{var i;var prepend=arguments[2]?String(arguments[2]):'';var append=arguments[3]?String(arguments[3]):'';var output='';var decimal='';var decimal_seperator=obj.Get('chart.scale.point')?obj.Get('chart.scale.point'):'.';var thousand_seperator=obj.Get('chart.scale.thousand')?obj.Get('chart.scale.thousand'):',';RegExp.$1='';var i,j;if(typeof(obj.Get('chart.scale.formatter'))=='function'){return obj.Get('chart.scale.formatter')(obj,num);}
if(String(num).indexOf('e')>0){return String(prepend+String(num)+append);}
num=String(num);if(num.indexOf('.')>0){var tmp=num;num=num.replace(/\.(.*)/,'');decimal=tmp.replace(/(.*)\.(.*)/,'$2');}
var seperator=thousand_seperator;var foundPoint;for(i=(num.length-1),j=0;i>=0;j++,i--){var character=num.charAt(i);if(j%3==0&&j!=0){output+=seperator;}
output+=character;}
var rev=output;output='';for(i=(rev.length-1);i>=0;i--){output+=rev.charAt(i);}
if(output.indexOf('-'+obj.Get('chart.scale.thousand'))==0){output='-'+output.substr(('-'+obj.Get('chart.scale.thousand')).length);}
if(decimal.length){output=output+decimal_seperator+decimal;decimal='';RegExp.$1='';}
if(output.charAt(0)=='-'){output=output.replace(/-/,'');prepend='-'+prepend;}
return prepend+output+append;}
RGraph.DrawBars=function(obj)
{var hbars=obj.Get('chart.background.hbars');obj.context.beginPath();for(i=0;i<hbars.length;++i){if(hbars[i][1]==null){hbars[i][1]=obj.max;}else if(hbars[i][0]+hbars[i][1]>obj.max){hbars[i][1]=obj.max-hbars[i][0];}
if(Math.abs(hbars[i][1])>obj.max){hbars[i][1]=-1*obj.max;}
if(Math.abs(hbars[i][0])>obj.max){hbars[i][0]=obj.max;}
if(hbars[i][0]+hbars[i][1]<(-1*obj.max)){hbars[i][1]=-1*(obj.max+hbars[i][0]);}
if(obj.Get('chart.xaxispos')=='bottom'&&(hbars[i][0]<0||(hbars[i][1]+hbars[i][1]<0))){alert('['+obj.type.toUpperCase()+' (ID: '+obj.id+') BACKGROUND HBARS] You have a negative value in one of your background hbars values, whilst the X axis is in the center');}
var ystart=(obj.grapharea-(((hbars[i][0]-obj.min)/(obj.max-obj.min))*obj.grapharea));var height=(Math.min(hbars[i][1],obj.max-hbars[i][0])/(obj.max-obj.min))*obj.grapharea;if(obj.Get('chart.xaxispos')=='center'){ystart/=2;height/=2;}
ystart+=obj.Get('chart.gutter.top')
var x=obj.Get('chart.gutter.left');var y=ystart-height;var w=obj.canvas.width-obj.Get('chart.gutter.left')-obj.Get('chart.gutter.right');var h=height;if(navigator.userAgent.indexOf('Opera')!=-1&&obj.Get('chart.xaxispos')=='center'&&h<0){h*=-1;y=y-h;}
if(obj.Get('chart.xaxispos')=='top'){y=obj.canvas.height-y;h*=-1;}
obj.context.fillStyle=hbars[i][2];obj.context.fillRect(x,y,w,h);}
obj.context.fill();}
RGraph.DrawInGraphLabels=function(obj)
{var canvas=obj.canvas;var context=obj.context;var labels=obj.Get('chart.labels.ingraph');var labels_processed=[];var fgcolor='black';var bgcolor='white';var direction=1;if(!labels){return;}
for(var i=0;i<labels.length;++i){if(typeof(labels[i])=='number'){for(var j=0;j<labels[i];++j){labels_processed.push(null);}}else if(typeof(labels[i])=='string'||typeof(labels[i])=='object'){labels_processed.push(labels[i]);}else{labels_processed.push('');}}
RGraph.NoShadow(obj);if(labels_processed&&labels_processed.length>0){for(var i=0;i<labels_processed.length;++i){if(labels_processed[i]){var coords=obj.coords[i];if(coords&&coords.length>0){var x=(obj.type=='bar'?coords[0]+(coords[2]/2):coords[0]);var y=(obj.type=='bar'?coords[1]+(coords[3]/2):coords[1]);var length=typeof(labels_processed[i][4])=='number'?labels_processed[i][4]:25;context.beginPath();context.fillStyle='black';context.strokeStyle='black';if(obj.type=='bar'){if(obj.Get('chart.xaxispos')=='top'){length*=-1;}
if(obj.Get('chart.variant')=='dot'){context.moveTo(AA(obj,x),obj.coords[i][1]-5);context.lineTo(AA(obj,x),obj.coords[i][1]-5-length);var text_x=AA(obj,x);var text_y=obj.coords[i][1]-5-length;}else if(obj.Get('chart.variant')=='arrow'){context.moveTo(AA(obj,x),obj.coords[i][1]-5);context.lineTo(AA(obj,x),obj.coords[i][1]-5-length);var text_x=AA(obj,x);var text_y=obj.coords[i][1]-5-length;}else{context.arc(AA(obj,x),y,2.5,0,6.28,0);context.moveTo(AA(obj,x),y);context.lineTo(AA(obj,x),y-length);var text_x=AA(obj,x);var text_y=y-length;}
context.stroke();context.fill();}else if(obj.type=='line'){if(typeof(labels_processed[i])=='object'&&typeof(labels_processed[i][3])=='number'&&labels_processed[i][3]==-1){context.moveTo(AA(obj,x),y+5);context.lineTo(AA(obj,x),y+5+length);context.stroke();context.beginPath();context.moveTo(AA(obj,x),y+5);context.lineTo(AA(obj,x)-3,y+10);context.lineTo(AA(obj,x)+3,y+10);context.closePath();var text_x=x;var text_y=y+5+length;}else{var text_x=x;var text_y=y-5-length;context.moveTo(AA(obj,x),y-5);context.lineTo(AA(obj,x),y-5-length);context.stroke();context.beginPath();context.moveTo(AA(obj,x),y-5);context.lineTo(AA(obj,x)-3,y-10);context.lineTo(AA(obj,x)+3,y-10);context.closePath();}
context.fill();}
context.beginPath();context.fillStyle=(typeof(labels_processed[i])=='object'&&typeof(labels_processed[i][1])=='string')?labels_processed[i][1]:'black';RGraph.Text(context,obj.Get('chart.text.font'),obj.Get('chart.text.size'),text_x,text_y,(typeof(labels_processed[i])=='object'&&typeof(labels_processed[i][0])=='string')?labels_processed[i][0]:labels_processed[i],'bottom','center',true,null,(typeof(labels_processed[i])=='object'&&typeof(labels_processed[i][2])=='string')?labels_processed[i][2]:'white');context.fill();}}}}}
RGraph.FixEventObject=function(e)
{if(RGraph.isIE8()||RGraph.isIE7()){var e=event;e.pageX=(event.clientX+document.body.scrollLeft);e.pageY=(event.clientY+document.body.scrollTop);e.target=event.srcElement;if(!document.body.scrollTop&&document.documentElement.scrollTop){e.pageX+=parseInt(document.documentElement.scrollLeft);e.pageY+=parseInt(document.documentElement.scrollTop);}}
if(typeof(e.offsetX)=='undefined'&&typeof(e.offsetY)=='undefined'){var coords=RGraph.getMouseXY(e);e.offsetX=coords[0];e.offsetY=coords[1];}
if(!e.stopPropagation){e.stopPropagation=function(){window.event.cancelBubble=true;}}
return e;}
RGraph.HideCrosshairCoords=function()
{var div=RGraph.Registry.Get('chart.coordinates.coords.div');if(div&&div.style.opacity==1&&div.__object__.Get('chart.crosshairs.coords.fadeout')){setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.9;},50);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.8;},100);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.7;},150);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.6;},200);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.5;},250);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.4;},300);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.3;},350);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.2;},400);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0.1;},450);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.opacity=0;},500);setTimeout(function(){RGraph.Registry.Get('chart.coordinates.coords.div').style.display='none';},550);}}
RGraph.Draw3DAxes=function(obj)
{var gutterLeft=obj.Get('chart.gutter.left');var gutterRight=obj.Get('chart.gutter.right');var gutterTop=obj.Get('chart.gutter.top');var gutterBottom=obj.Get('chart.gutter.bottom');var context=obj.context;var canvas=obj.canvas;context.strokeStyle='#aaa';context.fillStyle='#ddd';context.beginPath();context.moveTo(gutterLeft,gutterTop);context.lineTo(gutterLeft+10,gutterTop-5);context.lineTo(gutterLeft+10,canvas.height-gutterBottom-5);context.lineTo(gutterLeft,canvas.height-gutterBottom);context.closePath();context.stroke();context.fill();context.beginPath();context.moveTo(gutterLeft,canvas.height-gutterBottom);context.lineTo(gutterLeft+10,canvas.height-gutterBottom-5);context.lineTo(canvas.width-gutterRight+10,canvas.height-gutterBottom-5);context.lineTo(canvas.width-gutterRight,canvas.height-gutterBottom);context.closePath();context.stroke();context.fill();}
RGraph.OldBrowserCompat=function(context)
{if(!context){return;}
if(!context.measureText){context.measureText=function(text)
{var textObj=document.createElement('DIV');textObj.innerHTML=text;textObj.style.backgroundColor='white';textObj.style.position='absolute';textObj.style.top=-100
textObj.style.left=0;document.body.appendChild(textObj);var width={width:textObj.offsetWidth};textObj.style.display='none';return width;}}
if(!context.fillText){context.fillText=function(text,targetX,targetY)
{return false;}}
if(!context.canvas.addEventListener){window.addEventListener=function(ev,func,bubble)
{return this.attachEvent('on'+ev,func);}
context.canvas.addEventListener=function(ev,func,bubble)
{return this.attachEvent('on'+ev,func);}}}
RGraph.strokedCurvyRect=function(context,x,y,w,h)
{var r=arguments[5]?arguments[5]:3;var corner_tl=(arguments[6]||arguments[6]==null)?true:false;var corner_tr=(arguments[7]||arguments[7]==null)?true:false;var corner_br=(arguments[8]||arguments[8]==null)?true:false;var corner_bl=(arguments[9]||arguments[9]==null)?true:false;context.beginPath();context.moveTo(x+(corner_tl?r:0),y);context.lineTo(x+w-(corner_tr?r:0),y);if(corner_tr){context.arc(x+w-r,y+r,r,PI+HALFPI,TWOPI,false);}
context.lineTo(x+w,y+h-(corner_br?r:0));if(corner_br){context.arc(x+w-r,y-r+h,r,TWOPI,HALFPI,false);}
context.lineTo(x+(corner_bl?r:0),y+h);if(corner_bl){context.arc(x+r,y-r+h,r,HALFPI,PI,false);}
context.lineTo(x,y+(corner_tl?r:0));if(corner_tl){context.arc(x+r,y+r,r,PI,PI+HALFPI,false);}
context.stroke();}
RGraph.filledCurvyRect=function(context,x,y,w,h)
{var r=arguments[5]?arguments[5]:3;var corner_tl=(arguments[6]||arguments[6]==null)?true:false;var corner_tr=(arguments[7]||arguments[7]==null)?true:false;var corner_br=(arguments[8]||arguments[8]==null)?true:false;var corner_bl=(arguments[9]||arguments[9]==null)?true:false;context.beginPath();if(corner_tl){context.moveTo(x+r,y+r);context.arc(x+r,y+r,r,PI,PI+HALFPI,false);}else{context.fillRect(x,y,r,r);}
if(corner_tr){context.moveTo(x+w-r,y+r);context.arc(x+w-r,y+r,r,PI+HALFPI,0,false);}else{context.moveTo(x+w-r,y);context.fillRect(x+w-r,y,r,r);}
if(corner_br){context.moveTo(x+w-r,y+h-r);context.arc(x+w-r,y-r+h,r,0,HALFPI,false);}else{context.moveTo(x+w-r,y+h-r);context.fillRect(x+w-r,y+h-r,r,r);}
if(corner_bl){context.moveTo(x+r,y+h-r);context.arc(x+r,y-r+h,r,HALFPI,PI,false);}else{context.moveTo(x,y+h-r);context.fillRect(x,y+h-r,r,r);}
context.fillRect(x+r,y,w-r-r,h);context.fillRect(x,y+r,r+1,h-r-r);context.fillRect(x+w-r-1,y+r,r+1,h-r-r);context.fill();}
RGraph.HideZoomedCanvas=function()
{var interval=15;var frames=10;if(typeof(__zoomedimage__)=='object'){obj=__zoomedimage__.obj;}else{return;}
if(obj.Get('chart.zoom.fade.out')){for(var i=frames,j=1;i>=0;--i,++j){if(typeof(__zoomedimage__)=='object'){setTimeout("__zoomedimage__.style.opacity = "+String(i/10),j*interval);}}
if(typeof(__zoomedbackground__)=='object'){setTimeout("__zoomedbackground__.style.opacity = "+String(i/frames),j*interval);}}
if(typeof(__zoomedimage__)=='object'){setTimeout("__zoomedimage__.style.display = 'none'",obj.Get('chart.zoom.fade.out')?(frames*interval)+10:0);}
if(typeof(__zoomedbackground__)=='object'){setTimeout("__zoomedbackground__.style.display = 'none'",obj.Get('chart.zoom.fade.out')?(frames*interval)+10:0);}}
RGraph.AddCustomEventListener=function(obj,name,func)
{if(typeof(RGraph.events[obj.uid])=='undefined'){RGraph.events[obj.uid]=[];}
RGraph.events[obj.uid].push([obj,name,func]);return RGraph.events[obj.uid].length-1;}
RGraph.FireCustomEvent=function(obj,name)
{if(obj&&obj.isRGraph){var uid=obj.uid;if(typeof(uid)=='string'&&typeof(RGraph.events)=='object'&&typeof(RGraph.events[uid])=='object'&&RGraph.events[uid].length>0){for(var j=0;j<RGraph.events[uid].length;++j){if(RGraph.events[uid][j]&&RGraph.events[uid][j][1]==name){RGraph.events[uid][j][2](obj);}}}}}
RGraph.getGutterSuggest=function(obj,data)
{var min=0;for(var i=0;i<data.length;++i){min=Math.min(min,data[i]);}
var min=Math.abs(min);var str=RGraph.number_format(obj,RGraph.array_max(RGraph.getScale(Math.max(min,RGraph.array_max(data)),obj)),obj.Get('chart.units.pre'),obj.Get('chart.units.post'));if(obj.type=='hbar'){var str='';var len=0;for(var i=0;i<obj.Get('chart.labels').length;++i){str=(obj.Get('chart.labels').length>str.length?obj.Get('chart.labels')[i]:str);}}
obj.context.font=obj.Get('chart.text.size')+'pt '+obj.Get('chart.text.font');len=obj.context.measureText(str).width+5;return(obj.type=='hbar'?len/3:len);}
RGraph.SetConfig=function(obj,c)
{for(i in c){if(typeof(i)=='string'){obj.Set(i,c[i]);}}
return obj;}
RGraph.RemoveAllCustomEventListeners=function()
{var id=arguments[0];if(id&&RGraph.events[id]){RGraph.events[id]=[];}else{RGraph.events=[];}}
RGraph.RemoveCustomEventListener=function(obj,i)
{if(typeof(RGraph.events)=='object'&&typeof(RGraph.events[obj.id])=='object'&&typeof(RGraph.events[obj.id][i])=='object'){RGraph.events[obj.id][i]=null;}}
RGraph.DrawBackgroundImage=function(obj)
{if(typeof(obj.Get('chart.background.image'))=='string'){if(typeof(obj.canvas.__rgraph_background_image__)=='undefined'){var img=new Image();img.__object__=obj;img.__canvas__=obj.canvas;img.__context__=obj.context;img.src=obj.Get('chart.background.image');obj.canvas.__rgraph_background_image__=img;}else{img=obj.canvas.__rgraph_background_image__;}
img.onload=function()
{RGraph.Clear(obj.canvas);RGraph.RedrawCanvas(obj.canvas);}
var gutterLeft=obj.Get('chart.gutter.left');var gutterRight=obj.Get('chart.gutter.right');var gutterTop=obj.Get('chart.gutter.top');var gutterBottom=obj.Get('chart.gutter.bottom');var stretch=obj.Get('chart.background.image.stretch');var align=obj.Get('chart.background.image.align');if(typeof(align)=='string'){if(align.indexOf('right')!=-1){var x=obj.canvas.width-img.width-gutterRight;}else{var x=gutterLeft;}
if(align.indexOf('bottom')!=-1){var y=obj.canvas.height-img.height-gutterBottom;}else{var y=gutterTop;}}else{var x=gutterLeft;var y=gutterTop;}
var x=typeof(obj.Get('chart.background.image.x'))=='number'?obj.Get('chart.background.image.x'):x;var y=typeof(obj.Get('chart.background.image.y'))=='number'?obj.Get('chart.background.image.y'):y;var w=stretch?obj.canvas.width-gutterLeft-gutterRight:img.width;var h=stretch?obj.canvas.height-gutterTop-gutterBottom:img.height;if(typeof(obj.Get('chart.background.image.w'))=='number')w=obj.Get('chart.background.image.w');if(typeof(obj.Get('chart.background.image.h'))=='number')h=obj.Get('chart.background.image.h');obj.context.drawImage(img,x,y,w,h);}}
RGraph.hasTooltips=function(obj)
{if(typeof(obj.Get('chart.tooltips'))=='object'&&obj.Get('chart.tooltips')){for(var i=0;i<obj.Get('chart.tooltips').length;++i){if(!RGraph.is_null(obj.Get('chart.tooltips')[i])){return true;}}}else if(typeof(obj.Get('chart.tooltips'))=='function'){return true;}
return false;}
RGraph.CreateUID=function()
{return'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g,function(c)
{var r=Math.random()*16|0,v=c=='x'?r:(r&0x3|0x8);return v.toString(16);});}
RGraph.ObjectRegistry.Add=function(obj)
{var uid=obj.uid;var canvasID=obj.canvas.id;RGraph.ObjectRegistry.objects.byUID.push([uid,obj]);RGraph.ObjectRegistry.objects.byCanvasID.push([canvasID,obj]);}
RGraph.ObjectRegistry.Remove=function(obj)
{var id=obj.id;var uid=obj.uid;for(var i=0;i<RGraph.ObjectRegistry.objects.byUID.length;++i){if(RGraph.ObjectRegistry.objects.byUID[i]&&RGraph.ObjectRegistry.objects.byUID[i][1].uid==uid){RGraph.ObjectRegistry.objects.byUID[i]=null;}}
for(var i=0;i<RGraph.ObjectRegistry.objects.byCanvasID.length;++i){if(RGraph.ObjectRegistry.objects.byCanvasID[i]&&RGraph.ObjectRegistry.objects.byCanvasID[i][0]==id){RGraph.ObjectRegistry.objects.byCanvasID[i]=null;}}}
RGraph.ObjectRegistry.Clear=function()
{if(arguments[0]){var id=(typeof(arguments[0])=='object'?arguments[0].id:arguments[0]);var objects=RGraph.ObjectRegistry.getObjectsByCanvasID(id);for(var i=0;i<objects.length;++i){RGraph.ObjectRegistry.Remove(objects[i]);}}else{RGraph.ObjectRegistry.objects={};RGraph.ObjectRegistry.objects.byUID=[];RGraph.ObjectRegistry.objects.byCanvasID=[];}}
RGraph.ObjectRegistry.getObjectsByCanvasID=function(id)
{var store=RGraph.ObjectRegistry.objects.byCanvasID;var ret=[];for(var i=0;i<store.length;++i){if(store[i]&&store[i][0]==id){ret.push(store[i][1]);}}
return ret;}
RGraph.ObjectRegistry.getFirstObjectByXY=RGraph.ObjectRegistry.getObjectByXY=function(e)
{var canvas=e.target;var ret=null;var objects=RGraph.ObjectRegistry.getObjectsByCanvasID(canvas.id);for(var i=(objects.length-1);i>=0;--i){var obj=objects[i].getObjectByXY(e);if(obj){return obj;}}}
RGraph.ObjectRegistry.getObjectsByXY=function(e)
{var canvas=e.target;var ret=[];var objects=RGraph.ObjectRegistry.getObjectsByCanvasID(canvas.id);for(var i=(objects.length-1);i>=0;--i){var obj=objects[i].getObjectByXY(e);if(obj){ret.push(obj);}}
return ret;}
RGraph.ObjectRegistry.getObjectByUID=function(uid)
{var objects=RGraph.ObjectRegistry.objects.byUID;for(var i=0;i<objects.length;++i){if(objects[i]&&objects[i][1].uid==uid){return objects[i][1];}}}
RGraph.ObjectRegistry.getObjectsByType=function(canvas,type)
{if(typeof(canvas)=='string'){canvas=document.getElementById(canvas);}
var objects=RGraph.ObjectRegistry.getObjectsByCanvasID(canvas.id);var ret=[];for(var i=0;i<objects.length;++i){if(objects[i]&&objects[i].type&&objects[i].type==type){ret.push(objects[i]);}}
return ret;}
RGraph.ObjectRegistry.getFirstObjectByType=function(canvas,type)
{var objects=RGraph.ObjectRegistry.getObjectsByType(canvas,type);return objects.length>0?objects[0]:null;}
RGraph.getAngleByXY=function(cx,cy,x,y)
{var angle=Math.atan((y-cy)/(x-cx));angle=Math.abs(angle)
if(x>=cx&&y>=cy){angle+=TWOPI;}else if(x>=cx&&y<cy){angle=(HALFPI-angle)+(PI+HALFPI);}else if(x<cx&&y<cy){angle+=PI;}else{angle=PI-angle;}
if(angle>TWOPI){angle-=TWOPI;}
return angle;}
RGraph.getHypLength=function(x1,y1,x2,y2)
{var ret=Math.sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));return ret;}
RGraph.InstallEventListeners=function(obj)
{if(RGraph.isOld()){return;}
if(RGraph.InstallCanvasClickListener){RGraph.InstallWindowMousedownListener(obj);RGraph.InstallWindowMouseupListener(obj);RGraph.InstallCanvasMousemoveListener(obj);RGraph.InstallCanvasMouseupListener(obj);RGraph.InstallCanvasMousedownListener(obj);RGraph.InstallCanvasClickListener(obj);}else if(RGraph.hasTooltips(obj)||obj.Get('chart.adjustable')||obj.Get('chart.annotatable')||obj.Get('chart.contextmenu')||obj.Get('chart.resizable')||obj.Get('chart.key.interactive')||obj.Get('chart.events.click')||obj.Get('chart.events.mousemove')){alert('[RGRAPH] You appear to have used dynamic features but not included the file: RGraph.common.dynamic.js');}}
RGraph.pr=function(obj)
{var str='';var indent=(arguments[2]?arguments[2]:'');switch(typeof(obj)){case'number':if(indent==''){str+='Number: '}
str+=String(obj);break;case'string':if(indent==''){str+='String ('+obj.length+'):'}
str+='"'+String(obj)+'"';break;case'object':if(obj==null){str+='null';break;}
str+='Object\n'+indent+'(\n';for(var i in obj){if(typeof(i)=='string'||typeof(i)=='number'){str+=indent+' '+i+' => '+RGraph.pr(obj[i],true,indent+'    ')+'\n';}}
var str=str+indent+')';break;case'function':str+=obj;break;case'boolean':str+='Boolean: '+(obj?'true':'false');break;}
if(arguments[1]){return str;}else{alert(str);}}
RGraph.DashedLine=function(context,x1,y1,x2,y2)
{var size=5;if(typeof(arguments[5])=='number'){size=arguments[5];}
var dx=x2-x1;var dy=y2-y1;var num=Math.floor(Math.sqrt((dx*dx)+(dy*dy))/size);var xLen=dx/num;var yLen=dy/num;var count=0;do{count%2==0?context.lineTo(x1,y1):context.moveTo(x1,y1);x1+=xLen;y1+=yLen;}while(count++<num);}
RGraph.AJAX=function(url,callback)
{if(window.XMLHttpRequest){var httpRequest=new XMLHttpRequest();}else if(window.ActiveXObject){var httpRequest=new ActiveXObject("Microsoft.XMLHTTP");}
httpRequest.onreadystatechange=function()
{if(this.readyState==4&&this.status==200){this.__user_callback__=callback;this.__user_callback__();}}
httpRequest.open('GET',url,true);httpRequest.send();}
RGraph.RotateCanvas=function(canvas,x,y,angle)
{var context=canvas.getContext('2d');context.translate(x,y);context.rotate(angle);context.translate(0-x,0-y);}
function AA(obj,value){var value=String(value).replace(/^(\d+)\.\d+/,'$1');var newvalue=Number(value)+0.5;return(newvalue-value)>=0?newvalue:Math.floor(value);}
RGraph.LinearGradient=function(obj,x1,y1,x2,y2,color1,color2){var gradient=obj.context.createLinearGradient(x1,y1,x2,y2);gradient.addColorStop(0,color1);gradient.addColorStop(1,color2);return gradient;}
RGraph.RadialGradient=function(obj,x1,y1,r1,x2,y2,r2,color1,color2){var gradient=obj.context.createRadialGradient(x1,y1,r1,x2,y2,r2);gradient.addColorStop(0,color1);gradient.addColorStop(1,color2);return gradient;}
RGraph.array_shift=function(arr){var ret=[];for(var i=1;i<arr.length;++i){ret.push(arr[i]);}return ret;}
RGraph.AddEventListener=function(id,e,func){var type=arguments[3]?arguments[3]:'unknown';RGraph.Registry.Get('chart.event.handlers').push([id,e,func,type]);}
RGraph.ClearEventListeners=function(id){}
RGraph.HidePalette=function(){var div=RGraph.Registry.Get('palette');if(typeof(div)=='object'&&div){div.style.visibility='hidden';div.style.display='none';RGraph.Registry.Set('palette',null);}}
RGraph.random=function(min,max){var dp=arguments[2]?arguments[2]:0;var r=Math.random();return Number((((max-min)*r)+min).toFixed(dp));}
RGraph.NoShadow=function(obj){obj.context.shadowColor='rgba(0,0,0,0)';obj.context.shadowBlur=0;obj.context.shadowOffsetX=0;obj.context.shadowOffsetY=0;}
RGraph.SetShadow=function(obj,color,offsetx,offsety,blur){obj.context.shadowColor=color;obj.context.shadowOffsetX=offsetx;obj.context.shadowOffsetY=offsety;obj.context.shadowBlur=blur;}
RGraph.array_reverse=function(arr){var newarr=[];for(var i=arr.length-1;i>=0;i--){newarr.push(arr[i]);}return newarr;}
RGraph.Registry.Set=function(name,value){RGraph.Registry.store[name]=value;return value;}
RGraph.Registry.Get=function(name){return RGraph.Registry.store[name];}
RGraph.degrees2Radians=function(degrees){return degrees*(PI/180);}
RGraph.is_array=function(obj){return obj!=null&&obj.constructor.toString().indexOf('Array')!=-1;}
RGraph.trim=function(str){return RGraph.ltrim(RGraph.rtrim(str));}
RGraph.ltrim=function(str){return str.replace(/^(\s|\0)+/,'');}
RGraph.rtrim=function(str){return str.replace(/(\s|\0)+$/,'');}
RGraph.GetHeight=function(obj){return obj.canvas.height;}
RGraph.GetWidth=function(obj){return obj.canvas.width;}
RGraph.is_null=function(arg){if(arg==null||(typeof(arg))=='object'&&!arg){return true;}return false;}
RGraph.Timer=function(label){var d=new Date();console.log(label+': '+d.getSeconds()+'.'+d.getMilliseconds());}
RGraph.Async=function(func){return setTimeout(func,arguments[1]?arguments[1]:1);}
RGraph.isIE7=function(){return navigator.userAgent.indexOf('MSIE 7')>0;}
RGraph.isIE8=function(){return navigator.userAgent.indexOf('MSIE 8')>0;}
RGraph.isIE9=function(){return navigator.userAgent.indexOf('MSIE 9')>0;}
RGraph.isIE9up=function(){navigator.userAgent.match(/MSIE (\d+)/);return Number(RegExp.$1)>=9;}
RGraph.isOld=function(){return RGraph.isIE7()||RGraph.isIE8();}
RGraph.Reset=function(canvas){canvas.width=canvas.width;}
function pd(variable){RGraph.pr(variable);}
function p(variable){RGraph.pr(variable);}
function a(variable){alert(variable);}
function cl(variable){return console.log(variable);}