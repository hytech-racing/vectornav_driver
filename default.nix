{ src, pkgs, stdenv , cmake, vn_driver_lib_gen, boost, ... }:
stdenv.mkDerivation {
  pname = "vn_driver";
  version = "0.0.1";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  # buildInputs = [ boost ];
  propagatedBuildInputs = [ vn_driver_lib_gen boost ];
}