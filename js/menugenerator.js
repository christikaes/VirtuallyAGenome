$(function(){
    // Menu Items
    var geneList = ["CCDC140,PAX3,DD413687", "DUSP6", "HIF1A", "HIF1A,HIF1A-A52", "HMGB3", "IRX3", "KLF4", "LEFTY1", "NANOGNB,NANOG", "PRDM14", "PSORS1C3,POU5F1", "PYCR2,LEFTY1", "SMAD3", "SOX2", "SOX17", "TBX3", "IRX3", "NANOGNB,NANOG"]
    var colors = ["#f44336", "#E91E63", "#9C27B0", "#673AB7", "#3F51B5", "#2196F3", "#03A9F4", "#00BCD4", "#009688", "#4CAF50", "#8BC34A", "#CDDC39", "#FFEB3B", "#FFC107", "#FF9800", "#FF5722", "#795548", "#9E9E9E", "#607D8B"]

    var menuItems = "";
    for (var i = 0; i < geneList.length; i++) {
      var l = 9; // geneList.length / 2 round
      var x = i < l ? 1.5 : 4.5;
      var y = i%l/2 - 1;
      var plane = "<a-plane color='" + colors[i] + "' height='0.4' width='2.5' position='" + x + " " + y + " -3'></a-plane>"
      var text =  "<a-entity bmfont-text='text: " + geneList[i] + "; color: #000; lineHeight: 50' position='" + (x-1) + " " + y + " -2.99'></a-entity>"
      menuItems += plane;
      menuItems += text;
    }
    $("#scene").append(menuItems);
});
