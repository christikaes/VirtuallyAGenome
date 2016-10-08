$(function() {
    console.log( "Genome Editor Ready!" );

    var nodes = "";

    // Get the genome data and generate the nodes for the a-frame
    $.getJSON( "/data/genome1.json", function( data ) {
      var nodeData = data.nodes;
      for (var i = 0; i < nodeData.length; i++) {
        nodes += "<a-sphere position='" + i + " 1 0' radius='" + (nodeData[i].size/50) + "' color='" + (nodeData[i].color) + "'></a-sphere>"
      }
      $("#genome-viewer").append(nodes);
    });


});
