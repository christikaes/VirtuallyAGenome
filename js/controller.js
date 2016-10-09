$(function (){
    var prevNodeId;
    var prevNodeColor;

    var showGene = function(gene) {
      $(".3dgene").attr('visible', 'false');
      $("#" + gene).attr('visible', 'true');

      $(".2dgene").attr('visible', 'false');
      $("#" + gene + "-2d").attr('visible', 'true');
    }

    var selectMenu = function(id) {
      $(".menu").attr('color', '#3F51B5');
      $("#" + id).attr('color', '#2196F3');

      // Reset the color of the previous node
      $("#" + prevNodeId).attr('color', prevNodeColor);
    }

    var selectNode = function(id) {
      // Reset the color of the previous node
      $("#" + prevNodeId).attr('color', prevNodeColor);

      // Store this for later
      prevNodeId = id;
      prevNodeColor = $("#" + id).attr('color');

      // Show selected state
      $("#" + id).attr('color', '#E91E63');

      // TODO: Show in ui
      // Fill in data about node:
      var data = {
        name : $("#" + id).data('name'),
        id : $("#" + id).data('id'),
        mid : $("#" + id).data('mid'),
        fpkm : $("#" + id).data('fpkm'),
        tfbs : $("#" + id).data('tfbs')
      }

      console.table(data);
      $("#3d-genome-viewer-node").empty();
      var info = "<a-plane color='#E91E63' height='2' width='4' position='0 0 -3'></a-plane>";
      info += "<a-entity bmfont-text='text: Details; color: #CCDC140; lineHeight: 50' position='-1.9 0.6 -2.99'></a-entity>";
      info += "<a-entity bmfont-text='text: Name: " + data.name + "; color: #fff; lineHeight: 50' position='-1.9 0.2 -2.99'></a-entity>";
      info += "<a-entity bmfont-text='text: Id: " + data.id + "; color: #fff; lineHeight: 50' position='-1.9 0 -2.99'></a-entity>";
      info += "<a-entity bmfont-text='text: mid: " + data.mid + "; color: #fff; lineHeight: 50' position='-1.9 -0.2 -2.99'></a-entity>";
      info += "<a-entity bmfont-text='text: fpkm: " + data.fpkm + "; color: #fff; lineHeight: 50' position='-1.9 -0.4 -2.99'></a-entity>";
      // info += "<a-entity bmfont-text='text: tfbs: " + data.tfbs + "; color: #fff; lineHeight: 50' position='-1.9 -1.5 -2.99'></a-entity>";
      $("#3d-genome-viewer-node").append(info);
    }

    // When the user clicks on a menu, show the correct gene
    // Component to change to random color on click.
    AFRAME.registerComponent('cursor-listener', {
      init: function () {
        var geneList = ["DUSP6", "HMGB3", "LEFTY1", "PRDM14", "SOX2", "IRX3", "NANOG", "SMAD3", "TBX3", "HIF1A", "KLF4", "PAX3", "SOX17"]

        // Select the first gene
        showGene("TBX3");
        selectMenu("TBX3-menu");

        // for (var i = 0; i < geneList.length; i++) {
          $("#scene").on('click', function (evt) {
            console.log('click')
            var id = evt.target.id;
            var gene = id.split("-")[0];
            var component = id.split("-")[1];

            switch (component) {
              case "menu":
                showGene(gene);
                selectMenu(id);
                break;
              case "node":
                selectNode(id);
                break;
              default:
                console.log("error")
            }

          });
        // }

      }
    });



});
