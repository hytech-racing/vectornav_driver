{ pkgs, python311Packages, stdenv, py_vn_lib, ...}:

python311Packages.buildPythonApplication {
  pname = "vn_configurator";
  version = "0.0.1";
  propagatedBuildInputs = with python311Packages; [
    pyserial
    pyserial-asyncio
    py_vn_lib
  ];
  src = ./configurator;

}