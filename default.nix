{ src, pkgs, stdenv , cmake, vn_driver_lib_gen, ... }:
stdenv.mkDerivation {
  pname = "vn_driver";
  version = "0.0.1";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  propagatedBuildInputs = [ vn_driver_lib_gen ];
}