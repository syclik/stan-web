'use strict';

/* Controllers */

function AppCtrl($scope, $http) {
  /*$http({method: 'GET', url: '/api/name'}).
    success(function(data, status, headers, config) {
    $scope.name = data.name;
    }).
    error(function(data, status, headers, config) {
    $scope.name = 'Error!'
    });*/
}

function LinearModelCtrl($scope, $http, $timeout) {
  function tick() {
    $http({method: 'GET', url: '/api/sample'})
      .success(function(data, status, headers, config) {
	$scope.data = data;
      })
      .error(function(data, status, headers, config) {
	$scope.data = 'ERROR';
      });
    $timeout(tick, 100);
  };
  tick();
}


function MyCtrl1() {}
MyCtrl1.$inject = [];


function MyCtrl2() {
}
MyCtrl2.$inject = [];
