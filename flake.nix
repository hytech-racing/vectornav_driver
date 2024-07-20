{
  description = "my packages. im tired of making new repos for nix packages and im too lazy to push em up to nixpkgs";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-parts.url = "github:hercules-ci/flake-parts";
    devshell.url = "github:numtide/devshell";
    nebs-packages.url = "github:RCMast3r/nebs_packages";
  };
  outputs = { self, nixpkgs, flake-parts, devshell, nebs-packages, ... }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; }
      {
        systems = [
          "x86_64-linux"
          "aarch64-linux"
        ];
        imports = [
          inputs.flake-parts.flakeModules.easyOverlay
          inputs.devshell.flakeModule
        ];
        perSystem = { config, pkgs, system, ... }:
          let
            vn_driver_lib_gen_src = pkgs.callPackage ./vn_driver_lib_gen.nix { };
            vn_driver_lib_gen = pkgs.callPackage ./vn_driver_lib.nix { inherit vn_driver_lib_gen_src; };
            vn_driver = pkgs.callPackage ./default.nix { inherit vn_driver_lib_gen; };
          in
          {
            _module.args.pkgs = import inputs.nixpkgs {
              inherit system;
              overlays = [
                nebs-packages.overlays.default
              ];
              config = { };
            };
            packages.vn_driver_lib_gen = vn_driver_lib_gen;
            packages.vn_driver_lib_gen_src = vn_driver_lib_gen_src;
            packages.vn_driver = vn_driver;
            packages.default = vn_driver;
            overlayAttrs = {
              inherit (config.packages) default;
            };
            devshells.default = {
            env = [
              {
                name = "HTTP_PORT";
                value = 8080;
              }
            ];
            commands = [
              {
                help = "print hello";
                name = "hello";
                command = "echo hello";
              }
            ];
            packages = [
              pkgs.cowsay
              vn_driver_lib_gen
              pkgs.cmake
              pkgs.commslib
            ];
            packagesFrom = [ vn_driver ];
          };
            # legacyPackages =
            #   import nixpkgs {
            #     inherit system;
            #     overlays = [ (final: _: { flow-ipc = final.callPackage ./flow-ipc.nix { src = flow-ipc-src; }; }) ];
            #   };

          };
      };
}
