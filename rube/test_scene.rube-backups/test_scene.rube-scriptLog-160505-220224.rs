//Started script log at 05/05/16 21:47:53

getBody(3).select();
addBody(49, '{"angle":1.570796370506287,"angularVelocity":0,"awake":true,"linearVelocity":0,"name":"left incline","position":{"x":0,"y":5},"type":"static"}');
getBody(49).addFixture(49, '{"density":1,"friction":0.2,"name":"fixture1","shapes":[{"type":"polygon"}],"vertices":{"x":[-0.6281546950340271,0.6281514763832092,0.6281542181968689,-0.6281519532203674],"y":[-31.59878540039063,-31.59878730773926,31.59889793395996,31.59889984130859]}}');
getBody(3).deselect();
getBody(49).select();
getBody(49).setPosition(-149.317,84.6961);
getBody(49).setPosition(-134.255,56.5799);
getBody(49).deselect();
setCursor(-134.565, 63.2264);
addImage(1, '{"body":null,"center":{"x":"C30690B4","y":"427CE7E0"},"file":"C:/Users/daniel/Desktop/tree1.jpg","filter":1,"opacity":0.75,"scale":1}');
getImage(1).select();
getImage(1).setCenter(-134.565,63.2264);
getImage(1).setScale(17.327);
getImage(1).setCenter(-130.173,65.4225);
getImage(1).setCenter(-130.118,65.3398);
getImage(1).deselect();
setCursor(-133.712, 60.98);
addBody(50, '{"awake":true,"type":"dynamic"}');
getBody(50).addFixture(50, '{"density":1,"shapes":[{"radius":0,"type":"polygon"}],"friction":0.2,"vertices":{"x":[-0.5,0.5,0.5,-0.5],"y":[-0.5,-0.5,0.5,0.5]}}');
getBody(50).setPosition(-133.712,60.98);
getImage(1).select();
getImage(1).deselect();
getImage(1).delete();
addImage(2, '{"body":null,"center":{"x":"C305B630","y":"4273EB7B"},"file":"C:/Users/daniel/Desktop/tree1.png","filter":1,"opacity":0.25,"scale":1}');
getImage(2).select();
getImage(2).setCenter(-133.712,60.9799);
getImage(2).setScale(19.6359);
getImage(2).setCenter(-129.75,66.3017);
getBody(50).select();
getBody(50).setPosition(-133.562,63.1991);
getVertex(50,0).select();
getVertex(50,0).setPos(-1.97229, -5.94109);
getVertex(50,0).deselect();
getBody(50).deselect();
getBody(50).select();
getVertex(50,3).select();
getVertex(50,3).deselect();
getVertex(50,1).select();
getVertex(50,1).setPos(2.46306, -0.969425);
getVertex(50,1).setPos(1.20415, -5.94109);
getVertex(50,1).deselect();
getFixture(50).addVertex(2, 1.07505, -4.76015);
getFixture(50).addVertex(3, 0.893483, -3.09933);
getFixture(50).addVertex(4, 0.670177, -1.05667);
getFixture(50).addVertex(5, 0.546999, 0.070087);
getFixture(50).addVertex(8, -0.707492, -0.407754);
getFixture(50).addVertex(9, -1.10916, -2.165);
getFixture(50).addVertex(10, -1.51228, -3.92862);
getFixture(50).addVertex(11, -1.73821, -4.91704);
getFixture(50).addVertex(3, 0.991168, -3.9929);
getFixture(50).addVertex(4, 0.958523, -3.69428);
getFixture(50).addVertex(11, -0.876584, -1.14751);
getFixture(49).addVertex(4, -0.628153, -1.42099);
