{
    description = "Flake to build on NixOS.";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-24.05";
    };

    outputs =
        { nixpkgs, ... }:
        let
            system = "x86_64-linux";
            pkgs = nixpkgs.legacyPackages.${system};
        in
        {
            devShells.${system}.default = pkgs.mkShell {
                packages = [ pkgs.wine64 ];
                depsBuildBuild = [
                    (pkgs.pkgsCross.mingwW64.stdenv.cc.override ({
                        extraBuildCommands = ''
                            printf '%s' ' -L${pkgs.pkgsCross.mingwW64.windows.mcfgthreads}/lib' >> $out/nix-support/cc-ldflags
                            printf '%s' ' -I${pkgs.pkgsCross.mingwW64.windows.mcfgthreads.dev}/include' >> $out/nix-support/cc-cflags
                        '';
                    }))
                ];
            };
        };
}
