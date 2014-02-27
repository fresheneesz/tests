
// this sets the background color of the master UIView (when there are no windows/tab groups on it)
Titanium.UI.setBackgroundColor('#000');

var enbox = Titanium.UI.createWindow({  
    title:'Enbox',
    backgroundColor:'#fff'
});

var rows = []
for(var n=0; n<100; n++) {
	rows.push(enboxRow())
}

var list =  Titanium.UI.createTableView({data:rows});

enbox.add(list);
enbox.open()

function enboxRow() {
	var row = Titanium.UI.createTableViewRow({backgroundColor:"#33C"});
	row.add(Titanium.UI.createImageView({
		url:'Butterfly.gif',
		width:32,
		height:32,
		left:4,
		top:2
	}))
	row.add(Titanium.UI.createLabel({
		text: 'Someone\'s Name',
		font:{fontSize:16,fontWeight:'bold'},
		width:'auto',
		textAlign:'left',
		top:2,
		left:40,
		height:16
	}))
	row.add(imcount())
	
	/*row.add(emailcount())
	row.add(voicecount())
	row.add(socialcount())*/
	
	return row
	//return {className: 'enboxRow', title:"Test Row", backgroundColor:"#33C"};
}

function imcount() {
	var view = Titanium.UI.createView({
	   borderRadius:10,
	   width:50,
	   height:50
	});
	view.add(Titanium.UI.createImageView({
		url:'Butterfly.gif',
		width:32,
		height:32,
		left:4,
		top:2
	}))
	view.add(Titanium.UI.createLabel({
		text: '2',
		font:{fontSize:8,fontWeight:'bold'}
	}))
	
	return view
}
