var coordinates = AFRAME.utils.coordinates;
AFRAME.registerComponent('line', {
  schema: {
    color: { default: '#000' },
    path: {
      default: [
        { x: -0.5, y: 0, z: 0 },
        { x: 0.5, y: 0, z: 0 }
      ],

      // Deserialize path in the form of comma-separated vec3s: `0 0 0, 1 1 1, 2 0 3`.
      parse: function (value) {
        return value.split(',').map(coordinates.parse);
      },

      // Serialize array of vec3s in case someone does setAttribute('line', 'path', [...]).
      stringify: function (data) {
        return data.map(coordinates.stringify).join(',');
      }
    }
  },

  update: function () {
    var material = new THREE.LineBasicMaterial({
        linewidth: 2,
	      color: this.data.color
    });

    var geometry = new THREE.Geometry();
    this.data.path.forEach(function (vec3) {
      geometry.vertices.push(
        new THREE.Vector3(vec3.x, vec3.y, vec3.z)
      );
    });

    this.el.setObject3D('mesh', new THREE.Line(geometry, material));
  },

  remove: function () {
    this.el.removeObject3D('mesh');
  }
});
