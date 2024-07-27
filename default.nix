{ src, pkgs, stdenv , cmake, vn_driver_lib_gen, boost, easy_cmake, ... }:
stdenv.mkDerivation {
  pname = "vn_driver";
  version = "0.0.1";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ easy_cmake ];

  propagatedBuildInputs = [ vn_driver_lib_gen boost ];
}