$(function() {
    console.log( "Genome Editor Ready!" );


    // Get the genome data
    $.getJSON( "/data/genome1.json", function( data ) {
      // generate and add the nodes for the a-frame
      var nodes = "";
      var nodesData = data.nodes;
      for (var i = 0; i < nodesData.length; i++) {
        var multiplier = 1/50;
        var radius = nodesData[i].size/50;
        var color = nodesData[i].color;
        // ToDo: Replace with xyz
        var x = Math.floor(Math.random() * 10) + -10;
        var y = Math.floor(Math.random() * 10) + -5;
        var z = Math.floor(Math.random() * 10) + -10;
        nodesData[i].x = x;
        nodesData[i].y = y;
        nodesData[i].z = z;

        nodes += "<a-sphere position='" + x + " " + y + " " + z + "' radius='" + radius + "' color='" + color + "'></a-sphere>"
      }

      // generate and add the links for the a-frame
      var links = "";
      var linksData = data.links;
      for (var i = 0; i < linksData.length; i++) {
        var source = nodesData[linksData[i].source];
        var target = nodesData[linksData[i].target];
        var height = Math.sqrt(Math.pow((source.x-target.x), 2) + Math.pow((source.y-target.y), 2) + Math.pow((source.z-target.z), 2));

        links += "<a-entity line='color: black; path:" + source.x + " " + source.y + " " + source.z + ", " + target.x + " " + target.y + " " + target.z + "'></a-entity>"
      }

      $("#genome-viewer").append(nodes);
      $("#genome-viewer").append(links);
    });


});
