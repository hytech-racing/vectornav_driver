{
  description = "my packages. im tired of making new repos for nix packages and im too lazy to push em up to nixpkgs";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
    flake-parts.url = "github:hercules-ci/flake-parts";
    devshell.url = "github:numtide/devshell";
    nebs-packages.url = "github:RCMast3r/nebs_packages";
    easy_cmake.url = "github:RCMast3r/easy_cmake";
    vn_driver_lib.url = "github:RCMast3r/vn_driver_lib";
  };
  outputs = { self, nixpkgs, flake-parts, devshell, nebs-packages, easy_cmake, vn_driver_lib, ... }@inputs:

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
            vn_configurator = pkgs.callPackage ./vn_configurator.nix { };
          in
          {
            _module.args.pkgs = import inputs.nixpkgs {
              inherit system;
              overlays = [
                nebs-packages.overlays.default
                easy_cmake.overlays.default
                vn_driver_lib.overlays.default
              ];
              config = { };
            };
            packages.vn_driver_lib_gen = vn_driver_lib_gen;
            packages.vn_driver_lib_gen_src = vn_driver_lib_gen_src;
            packages.vn_driver = vn_driver;
            packages.vn_configurator = vn_configurator;

            packages.default = vn_driver;
            overlayAttrs = {
              inherit (config.packages) default vn_driver_lib_gen vn_driver;
            };
            devShells = {
              python = pkgs.mkShell {
                name = "nix-py-devshell";
                packages = vn_configurator.propagatedBuildInputs;
                # Setting up the environment variables you need during
                # development.
                shellHook =
                  let icon = "f121";
                  in ''
                    echo -e "PYTHONPATH=$PYTHONPATH\n" > .env
                  '';
              };

              devshell.default = {
                env = [ ];

                commands = [
                  {
                    name = "configure";
                    command = "${vn_configurator}/bin/write.py";
                  }
                  {
                    name = "b";
                    command = "cd $PRJ_ROOT && rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && make -j && cd $PRJ_ROOT";
                  }
                ];

                packages = [
                  pkgs.cmake
                  vn_configurator
                  pkgs.py_vn_lib
                  pkgs.python3
                ];

                packagesFrom = [ vn_driver vn_driver_lib_gen vn_configurator ];
              };
              # legacyPackages =
              #   import nixpkgs {
              #     inherit system;
              #     overlays = [ (final: _: { flow-ipc = final.callPackage ./flow-ipc.nix { src = flow-ipc-src; }; }) ];
              #   };
            };
          };
      };
}
