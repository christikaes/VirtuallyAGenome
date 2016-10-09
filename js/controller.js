$(function (){

    var showGene = function(gene) {
      $(".3dgene").attr('visible', 'false');
      $("#" + gene).attr('visible', 'true');

      $(".2dgene").attr('visible', 'false');
      $("#" + gene + "-2d").attr('visible', 'true');
    }

    // When the user clicks on a menu, show the correct gene
    // Component to change to random color on click.
    AFRAME.registerComponent('cursor-listener', {
      init: function () {
        console.log("cursor-listener")
        var geneList = ["DUSP6", "HMGB3", "LEFTY1", "PRDM14", "SOX2", "IRX3", "NANOG", "SMAD3", "TBX3", "HIF1A", "KLF4", "PAX3", "SOX17"]

        for (var i = 0; i < geneList.length; i++) {
          console.log("add listener")
          $("#" + geneList[i] + "-menu").on('click', function (evt) {
            console.log("click")
            var id = evt.target.id;
            var gene = id.split("-")[0];
            var component = id.split("-")[1];

            console.log(gene)
            console.log(component)

            switch (component) {
              case "menu":
                console.log("menu")
                showGene(gene);
                break;
              default:
                console.log("error")
            }

          });
        }

      }
    });



});
