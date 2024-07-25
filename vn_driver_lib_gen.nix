{ src, pkgs, stdenv , commsdsl, ... }:
stdenv.mkDerivation {
  pname = "vn_lib_gen";
  version = "0.0.1";
  src = ./dsl;
  propagatedBuildInputs = [ commsdsl ];
  buildPhase = ''
    ${commsdsl}/bin/commsdsl2comms main.xml
  '';

  installPhase = ''
    mkdir $out
    cp *.xml $out
    cp -r include $out
    cp -r doc $out
    cp CMakeLists.txt $out
  '';
}