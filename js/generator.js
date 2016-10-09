var generate3dGene = function (gene){
  // Get the gene data
  var dataUrl = "./data/" + gene + ".json";
  console.log(dataUrl)
  $.getJSON( dataUrl, function( data ) {
    console.log(data)
    // scale data
    var nodesData = data.nodes;
    for (var i = 0; i < nodesData.length; i++) {
      nodesData[i].size *= 1/50;
      nodesData[i].x -= 0.5;
      nodesData[i].y -= 0.5;
      nodesData[i].z -= 0.5;
      nodesData[i].x *= 2;
      nodesData[i].y *= 2;
      nodesData[i].z *= 2;

      switch (nodesData[i].color) {
        case "blue":
        nodesData[i].color = "#2196F3";
        break;
        case "purple":
        nodesData[i].color = "#9C27B0";
        break;
        case "orange":
        nodesData[i].color = "#FF9800";
        break;
        case "grey":
        nodesData[i].color = "#9E9E9E";
        break;
        default:
      }
    }

    // generate and add the nodes for the a-frame
    var nodes = "";
    for (var i = 0; i < nodesData.length; i++) {
      var radius = nodesData[i].size;
      var color = nodesData[i].color;
      var x = nodesData[i].x;
      var y = nodesData[i].y;
      var z = nodesData[i].z;

      nodes += "<a-sphere id='" + gene + "-" + i + "' position='" + x + " " + y + " " + z + "' radius='" + radius + "' color='" + color + "'></a-sphere>"
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

    var geneView = "<a-entity id='" + gene + "' class='3dgene'>" + nodes + links + "</a-entity>"
    $("#3d-genome-viewer").append(geneView);
  });
}

var generate2dGene = function (gene){
  // Get the gene data
  var imgUrl = "./img/genes/" + gene + ".jpg";
  var imgUrl = "./img/nucleus.jpg";
  var geneView = "<a-image id='" + gene + "-2d' class='2dgene' src='" + imgUrl + "'width='4' height='3'></a-image>"
  $("#2d-genome-viewer").append(geneView);
}

$(function() {
    // Generate the 3d genes
    var geneList = ["DUSP6", "HMGB3", "LEFTY1", "PRDM14", "SOX2", "IRX3", "NANOG", "SMAD3", "TBX3", "HIF1A", "KLF4", "PAX3", "SOX17"]

    for (var i = 0; i < geneList.length; i++) {
      console.log(geneList[i])
      generate3dGene(geneList[i]);
      generate2dGene(geneList[i]);
    }

    // Show only of them
    $(".3dgene").attr('visible', 'false');
    $("#DUSP6").attr('visible', 'true');
});
