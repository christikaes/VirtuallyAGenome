$(function(){
    console.log("menugenerator")
    var geneList = ["CCDC140,PAX3,DD413687", "222222222222222"]

    var menuItems = "";
    for (var i = 0; i < geneList.length; i++) {
      console.log(geneList[i])
      var plane = "<a-plane color='#CCC' height='0.4' width='3' position='1 " + i/2 + " -3'></a-plane>"
      var text =  "<a-entity bmfont-text='text: " + geneList[i] + "; color: #0ff; lineHeight: 50' position='0 " + i/2 + " -3'></a-entity>"
      menuItems += plane;
      menuItems += text;
    }
    console.log(menuItems)
    $("#scene").append(menuItems);
});
