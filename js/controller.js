$(function (){

    var showGene = function(gene) {
      $(".3dgene").attr('visible', 'false');
      $("#" + gene).attr('visible', 'true');

      $(".2dgene").attr('visible', 'false');
      $("#" + gene + "-2d").attr('visible', 'true');
    }

    var selectMenu = function(id) {
      $(".menu").attr('color', '#3F51B5');
      $("#" + id).attr('color', '#2196F3');
    }

    // When the user clicks on a menu, show the correct gene
    // Component to change to random color on click.
    AFRAME.registerComponent('cursor-listener', {
      init: function () {
        var geneList = ["DUSP6", "HMGB3", "LEFTY1", "PRDM14", "SOX2", "IRX3", "NANOG", "SMAD3", "TBX3", "HIF1A", "KLF4", "PAX3", "SOX17"]

        // Select the first gene
        showGene("DUSP6");
        selectMenu("DUSP6-menu");

        for (var i = 0; i < geneList.length; i++) {
          $("#" + geneList[i] + "-menu").on('click', function (evt) {
            var id = evt.target.id;
            var gene = id.split("-")[0];
            var component = id.split("-")[1];

            switch (component) {
              case "menu":
                showGene(gene);
                selectMenu(id);
                break;
              default:
                console.log("error")
            }

          });
        }

      }
    });



});
