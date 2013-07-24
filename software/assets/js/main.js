$(function() {
		var note_index =0;
		var index = 0;
		var a = ["C5", "B4", "A4", "G4", "F4", "E4", "D4", "C4"];
		for(i=0;i<a.length;i += 1){
				for(j=0;j<=7; j+=1){
						$("#selectable" + j).append("<li class='ui-widget-content'>"+a[i]+"</li>")
				}			
		}		
		
		$("#selectable8").append("<li class='ui-widget-content'>Play</li>");

		$( "#accordion" ).accordion({
				event: "mouseover"
		});		

		$("#selectable0").selectable({
				stop: function() {
						playAudio(0,0,0,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable0 li" ).index( this );
								playAudio(note,note_index,0,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		
		
		$("#selectable1").selectable({
				stop: function() {
						playAudio(0,0,1,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable1 li" ).index( this );
								playAudio(note,note_index,1,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable2").selectable({
				stop: function() {
						playAudio(0,0,2,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable2 li" ).index( this );
								playAudio(note,note_index,2,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable3").selectable({
				stop: function() {
						playAudio(0,0,3,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable3 li" ).index( this );
								playAudio(note,note_index,3,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable4").selectable({
				stop: function() {
						playAudio(0,0,4,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable4 li" ).index( this );
								playAudio(note,note_index,4,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable5").selectable({
				stop: function() {
						playAudio(0,0,5,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable5 li" ).index( this );
								playAudio(note,note_index,5,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable6").selectable({
				stop: function() {
						playAudio(0,0,6,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable6 li" ).index( this );
								playAudio(note,note_index,6,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable7").selectable({
				stop: function() {
						playAudio(0,0,7,8);
						$(".ui-selected",this).each(function() {
								var note = $(this).html();
								index = $( "#selectable7 li" ).index( this );
								playAudio(note,note_index,7,index);
								note_index +=1;
						});
						note_index = 0;
				}
		});	
		
		$("#selectable8").selectable({
				stop: sequencer
				
		});
});



window.onload = waveform;
//define wav_total as global so it can be used in waveform() and playAudio()
var wav_total = [];
var playNote;
var trigger_cnt = 0;
var sequence = new Array(8);
for (i=0;i<8;i++){
		sequence[i]=new Array(8);
}	
var sequence_tran = new Array(8);
for (i=0;i<8;i++){
		sequence_tran[i]=new Array(8);
}	

for (i=0;i<8;i++){
		for (j=0;j<8;j++){
				sequence_tran[i][j]=0;
		}
}

var current_state = 0;		

change_box_color = function(color) 
{
		
		dCanvas.style.backgroundColor = color;
		
}


bitstringToBMC = function (bitstring) {
    var bmc_array = "";

    var toggle = 0;

    $.each(bitstring, function(i, val) {
				if (val === "1") {
						if ( toggle === 0) {
								bmc_array += "01";
								toggle = 0;
						} else if (toggle === 1) {
								bmc_array += "10";
								toggle = 1;
						}
				} else if (val === "0") {
						if (toggle === 0) {
								bmc_array += "00";
								toggle = 1;
						} else if (toggle === 1) {
								bmc_array += "11";
								toggle = 0;
						}
				}
    });
    return bmc_array;
}

displayBMC = function (bmc_string, delay) {
    var i = 0;
    var lastTime = new Date();
    var first = 1;
    var adjustedDelay = 0;
		
    disp = function () {
				time = new Date();
				
				if (bmc_string[i] === "0") {
						change_box_color("#FFFFFF");
						
				} else if (bmc_string[i] == "1") {
						change_box_color("#000000");
				}
				
				if (i < bmc_string.length) {
						i += 1;
						if (first === 1) {
								adjustedDelay = delay;
								first = 0;
						} else {
								adjustedDelay = adjustedDelay - ((time-lastTime) - delay);
						}

						console.log(adjustedDelay); 

						timer = setTimeout(disp,adjustedDelay);
				}

				lastTime = time;
    };

    disp();
}	



function transmit(choice)
{ 
		var seq = "0011";
		var harmonics = "1100";
		var one_byte = "0001";
		var two_bytes = "0010";
		var three_bytes = "0011";
		var four_bytes = "0100";
		var five_bytes = "0101";
		var six_bytes = "0110";
		var seven_bytes = "0111";
		var eight_bytes = "1000";
		var fundamental = "000";
		var harmonic_1 = "001";
		var harmonic_2 = "010";
		var harmonic_3 = "011";
		var harmonic_4 = "100";
		var zero = "000";
		var quarter = "001";
		var half = "010";
		var three_quarters = "011";
		var whole = "100";
		var prologue = "00000001";
		var epilogue= "10000000";
		var bit_0;
		var bit_1;
		var bit_2;
		var bit_3;
		var bit_4;
		var bit_5;
		var bit_6;
		var bit_7;
		var seq_byte;
		var fund_amp;
		var harm1_amp;
		var harm2_amp;
		var harm3_amp;
		var harm4_amp;
		var opcode;
		var num_bytes;
		var choice;
		var transmit_string;
		var fund_int = document.getElementById("fund_int").value;
		var harm1_int = document.getElementById("harm1_int").value;
		var harm2_int = document.getElementById("harm2_int").value;
		var harm3_int = 0;
		var harm4_int = 0;
		
		
		if (choice == 1 ){
				
				if (fund_int == 0){
						fund_amp = zero;}
				else if(fund_int == 25){
						fund_amp = quarter;}
				else if(fund_int == 50){
						fund_amp = half;}
				else if(fund_int == 75){
						fund_amp = three_quarters;}
				else if(fund_int == 100){
						fund_amp = whole;}
				
				if (harm1_int == 0){
						harm1_amp = zero;}
				else if(harm1_int == 25){
						harm1_amp = quarter;}
				else if(harm1_int == 50){
						harm1_amp = half;}
				else if(harm1_int == 75){
						harm1_amp = three_quarters;}
				else if(harm1_int == 100){
						harm1_amp = whole;}
				
				if (harm2_int == 0){
						harm2_amp = zero;}
				else if(harm2_int == 25){
						harm2_amp = quarter;}
				else if(harm2_int == 50){
						harm2_amp = half;}
				else if(harm2_int == 75){
						harm2_amp = three_quarters;}
				else if(harm2_int == 100){
						harm2_amp = whole;}
				
				if (harm3_int == 0){
						harm3_amp = zero;}
				else if(harm3_int == 25){
						harm3_amp = quarter;}
				else if(harm3_int == 50){
						harm3_amp = half;}
				else if(harm3_int == 75){
						harm3_amp = three_quarters;}
				else if(harm3_int == 100){
						harm3_amp = whole;}
				
				if (harm4_int == 0){
						harm4_amp = zero;}
				else if(harm4_int == 25){
						harm4_amp = quarter;}
				else if(harm4_int == 50){
						harm4_amp = half;}
				else if(harm4_int == 75){
						harm4_amp = three_quarters;}
				else if(harm4_int == 100){
						harm4_amp = whole;}
				
				opcode = harmonics;
				transmit_string = prologue + opcode + five_bytes + fundamental + fund_amp + "00" + harmonic_1 + harm1_amp + "00" + harmonic_2 + harm2_amp + "00" + harmonic_3 + harm3_amp + "00" + harmonic_4 + harm4_amp + "00" + epilogue + "1";
				transmit_init(transmit_string);
		}
		

		
		else
		{
				for (i=0;i<8;i+=1){
						
						bit_0 = sequence_tran[i][0];
						bit_1 = sequence_tran[i][1];
						bit_2 = sequence_tran[i][2];
						bit_3 = sequence_tran[i][3];
						bit_4 = sequence_tran[i][4];
						bit_5 = sequence_tran[i][5];
						bit_6 = sequence_tran[i][6];
						bit_7 = sequence_tran[i][7];
						
						bit_0 = bit_0.toString();
						bit_1 = bit_1.toString();
						bit_2 = bit_2.toString();
						bit_3 = bit_3.toString();
						bit_4 = bit_4.toString();
						bit_5 = bit_5.toString();
						bit_6 = bit_6.toString();
						bit_7 = bit_7.toString();
						
						seq_byte = bit_0 + bit_1 + bit_2 + bit_3 + bit_4 + bit_5 + bit_6 + bit_7;
						
						if (i == 0){
								seq_0 = seq_byte;}
						else if (i==1){
								seq_1 = seq_byte;}
						else if (i==2){
								seq_2 = seq_byte;}
						else if (i==3){
								seq_3 = seq_byte;}
						else if (i==4){
								seq_4 = seq_byte;}
						else if (i==5){
								seq_5 = seq_byte;}
						else if (i==6){
								seq_6 = seq_byte;}
						else if (i==7){
								seq_7 = seq_byte;}
				}	
				opcode = seq;
				transmit_string = prologue + opcode + eight_bytes + seq_0 + seq_1 + seq_2 + seq_3 + seq_4 + seq_5 + seq_6 + seq_7 + epilogue + "1";
				
				transmit_init(transmit_string);
		}
		
		
}



function transmit_init(data){
		var delay = document.getElementById("delay").value;
		
		var bmc_string = bitstringToBMC(data);
		
		console.log(bmc_string)
		
		displayBMC(bmc_string, parseInt(delay));	
		
}

function waveform()
{
    var fund = [];
    var harm1 = [];
    var harm2 = [];
    var harm3 = [];
    var harm4 = [];
    var output_array = [];
    //max (and maybe min) used for normalizing at final stage
    var min; 
    var max; 
    //intensity multipliers from text boxes
		var fund_int = document.getElementById("fund_int").value;
    var harm1_int = document.getElementById("harm1_int").value;
    var harm2_int = document.getElementById("harm2_int").value;
		var harm3_int = 0
		var harm4_int = 0
    
    //initialize arrays to hold waveforms
		

		
		//populate arrays -- multiply full strength waveforms by intensity
    for (i=0; i<255; i++)
		{
				j=(i/255)*6.28;
				fund[i]  = Math.sin(j)*(fund_int/100);
				harm1[i] = Math.sin(2*j)*(harm1_int/100);
				harm2[i] = Math.sin(3*j)*(harm2_int/100);
				harm3[i] = Math.sin(4*j)*(harm3_int/100);
				harm4[i] = Math.sin(5*j)*(harm4_int/100);
				wav_total[i] = (fund[i]*(fund_int/100) + harm1[i]*(harm1_int/100) + harm2[i]*(harm2_int/100) + harm3[i]*(harm3_int/100) + harm4[i]*(harm4_int/100));
		}
		
		//min = Math.min.apply(Math,wav_total);
		
		//get maximum to normalize
		max = Math.max.apply(Math,wav_total);	
		//normalize
		for (i=0; i<255; i++){
				wav_total[i]= wav_total[i]*(1/(max));
		}
		//actual array going to synth
		for (i=0; i<255; i++){
				output_array[i]= Math.round((wav_total[i]+1)*2048);
		}
		var final = new RGraph.Line('final', wav_total);
		final.Set('chart.linewidth', 1);
		final.Set('chart.gutter.left', 35);
		final.Set('chart.hmargin', 5);
		final.Set('chart.ymin', -1);
		final.Set('chart.ymax', 1);
		final.Set('chart.outofbounds', true);
		final.Set('chart.numyticks', 0);
		final.Set('chart.numxticks', 0.0000001);
		final.Set('chart.colors', ['red', 'green']);
		final.Set('chart.scale.decimals', 1);
		final.Set('chart.curvy.factor', 0.25);
		final.Set('chart.background.grid.hlines', false);
		final.Set('chart.background.grid.vlines', false);
		
		//RGraph.isOld() ? final.Draw() : RGraph.Effects.Line.jQuery.Trace(final);
		RGraph.isOld() ? final.Draw() : RGraph.Effects.Fade.In(final,{'duration': 1});
		//final.Draw();
		
		var fund = new RGraph.Line('fund', fund);
		fund.Set('chart.linewidth', 1);
		fund.Set('chart.gutter.left', 35);
		fund.Set('chart.hmargin', 5);
		fund.Set('chart.ymin', -1);
		fund.Set('chart.ymax', 1);
		fund.Set('chart.outofbounds', true);
		fund.Set('chart.numyticks', 0);
		fund.Set('chart.numxticks', 0.0000001);
		fund.Set('chart.colors', ['red', 'green']);
		fund.Set('chart.scale.decimals', 1);
		fund.Set('chart.curvy.factor', 0.25);
		fund.Set('chart.background.grid.hlines', false);
		fund.Set('chart.background.grid.vlines', false);
		
		RGraph.isOld() ? fund.Draw() : RGraph.Effects.Fade.In(fund,{'duration': 1});
		
		var harm1 = new RGraph.Line('harm1', harm1);
		harm1.Set('chart.linewidth', 1);
		harm1.Set('chart.gutter.left', 35);
		harm1.Set('chart.hmargin', 5);
		harm1.Set('chart.ymin', -1);
		harm1.Set('chart.ymax', 1);
		harm1.Set('chart.outofbounds', true);
		harm1.Set('chart.numyticks', 0);
		harm1.Set('chart.numxticks', 0.0000001);
		harm1.Set('chart.colors', ['red', 'green']);
		harm1.Set('chart.scale.decimals', 1);
		harm1.Set('chart.curvy.factor', 0.25);
		harm1.Set('chart.background.grid.hlines', false);
		harm1.Set('chart.background.grid.vlines', false);
		
		RGraph.isOld() ? harm1.Draw() : RGraph.Effects.Fade.In(harm1,{'duration': 1});
		//harm1.reDraw();
		
		var harm2 = new RGraph.Line('harm2', harm2);
		harm2.Set('chart.linewidth', 1);
		harm2.Set('chart.gutter.left', 35);
		harm2.Set('chart.hmargin', 5);
		harm2.Set('chart.ymin', -1);
		harm2.Set('chart.ymax', 1);
		harm2.Set('chart.outofbounds', true);
		harm2.Set('chart.numyticks', 0);
		harm2.Set('chart.numxticks', 0.0000001);
		harm2.Set('chart.colors', ['red', 'green']);
		harm2.Set('chart.scale.decimals', 1);
		harm2.Set('chart.curvy.factor', 0.25);
		harm2.Set('chart.background.grid.hlines', false);
		harm2.Set('chart.background.grid.vlines', false);
		
		RGraph.isOld() ? harm2.Draw() : RGraph.Effects.Fade.In(harm2,{'duration': 1});
		
		var harm3 = new RGraph.Line('harm3', harm3);
		harm3.Set('chart.linewidth', 1);
		harm3.Set('chart.gutter.left', 35);
		harm3.Set('chart.hmargin', 5);
		harm3.Set('chart.ymin', -1);
		harm3.Set('chart.ymax', 1);
		harm3.Set('chart.outofbounds', true);
		harm3.Set('chart.numyticks', 0);
		harm3.Set('chart.numxticks', 0.0000001);
		harm3.Set('chart.colors', ['red', 'green']);
		harm3.Set('chart.scale.decimals', 1);
		harm3.Set('chart.curvy.factor', 0.25);
		harm3.Set('chart.background.grid.hlines', false);
		harm3.Set('chart.background.grid.vlines', false);
		
		RGraph.isOld() ? harm3.Draw() : RGraph.Effects.Fade.In(harm3,{'duration': 1});
		
		var harm4 = new RGraph.Line('harm4', harm4);
		harm4.Set('chart.linewidth', 1);
		harm4.Set('chart.gutter.left', 35);
		harm4.Set('chart.hmargin', 5);
		harm4.Set('chart.ymin', -1);
		harm4.Set('chart.ymax', 1);
		harm4.Set('chart.outofbounds', true);
		harm4.Set('chart.numyticks', 0);
		harm4.Set('chart.numxticks', 0.0000001);
		harm4.Set('chart.colors', ['red', 'green']);
		harm4.Set('chart.scale.decimals', 1);
		harm4.Set('chart.curvy.factor', 0.25);
		harm4.Set('chart.background.grid.hlines', false);
		harm4.Set('chart.background.grid.vlines', false);
		
		RGraph.isOld() ? harm4.Draw() : RGraph.Effects.Fade.In(harm4,{'duration': 1});
}

function ClearAndRedraw ()
{
		RGraph.ObjectRegistry.Clear() 
}	

function sequencer()
{
		setTimeout('timeout_trigger()', 400);
}


function timeout_trigger(){
		for (j=0;j<4;j+=1){
				
				playAudio((sequence[trigger_cnt][j]),0,10,0);
		}
		if (trigger_cnt == 7){
				trigger_cnt = 0;
		}
		else
		{
				trigger_cnt+=1;
				sequencer();
		}
}

function playAudio (note, note_index, column, index)
{
		var audio = new Audio(); // create the HTML5 audio element
		var wave = new RIFFWAVE(); // create an empty wave file
		
		var data = []; // yes, it's an array
		wave.header.sampleRate = 22000; // set sample rate to 22KHz
		wave.header.numChannels = 1; // one channels 
		
		if (column==10){  //sequencer
				playNote = note;
		}
		
		else if (column!=9)
		{
				
				if (note_index==0 | note_index==9){
						for (i=0;i<8;i+=1){
								sequence[column][i]=0;
								sequence_tran[column][i]=0;
						}
				}
				
				sequence[column][note_index]= note;
				console.log(sequence[0]);
				sequence_tran[column][index]=1;
				if(index!=8){
						playNote = note;
				}
		}
		var i = 0;
		var increment = 0;
		if(index!=8){
				while (i<10000) { 
						increment += notes[playNote];
						data[i++] = 128+Math.round(127*wav_total[(increment>>>notes.NOTES_BASE)%255]); // left speaker
				}
			  
				wave.Make(data); // make the wave file
				audio.src = wave.dataURI; // set audio source
				audio.play(); // we should hear two tones one on each speaker
		}
}
