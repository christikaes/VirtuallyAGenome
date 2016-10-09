$(function(){
    // Menu Items
    var geneList = ["DUSP6", "HMGB3", "LEFTY1", "PRDM14", "SOX2", "IRX3", "NANOG", "SMAD3", "TBX3", "HIF1A", "KLF4", "PAX3", "SOX17"]
    var colors = ["#f44336", "#E91E63", "#9C27B0", "#673AB7", "#3F51B5", "#2196F3", "#03A9F4", "#00BCD4", "#009688", "#4CAF50", "#8BC34A", "#CDDC39", "#FFEB3B", "#FFC107", "#FF9800", "#FF5722", "#795548", "#9E9E9E", "#607D8B"]

    var menuItems = "";
    for (var i = 0; i < geneList.length; i++) {
      var l = 7; // geneList.length / 2 round
      var x = i < l ? 1.5 : 4.5;
      var y = i%l/2;
      var plane = "<a-plane class='menu' id='" + geneList[i] + "-menu' color='" + colors[i] + "' height='0.4' width='2.5' position='" + x + " " + y + " -3'></a-plane>"
      var text =  "<a-entity bmfont-text='text: " + geneList[i] + "; color: #000; lineHeight: 50' position='" + (x-1) + " " + y + " -2.99'></a-entity>"
      menuItems += plane;
      menuItems += text;
    }
    $("#scene").append(menuItems);
});
