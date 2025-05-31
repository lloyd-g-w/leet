{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    clang-tools # for clangd
    cmake
    gcc
    glfw
    mesa
    pkg-config

    # Wayland + XKB
    wayland
    wayland-scanner
    wayland-protocols
    libxkbcommon

    # libffi → so wayland-client.pc can be found
    libffi

    # X11 core (using xorg.* attributes)
    xorg.libX11
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXi
    xorg.libXxf86vm
    xorg.libXrender

    # Vulkan loader + headers (→ satisfy FindVulkan)
    vulkan-loader
    vulkan-headers
  ];
  shellHook = ''
        export LD_LIBRARY_PATH=${pkgs.wayland}/lib:\
    ${pkgs.libxkbcommon}/lib:\
    ${pkgs.libffi}/lib:\
    ${pkgs.xorg.libxcb}/lib:\
    ${pkgs.xorg.libX11}/lib:\
    ${pkgs.xorg.libXcursor}/lib:\
    ${pkgs.xorg.libXrandr}/lib:\
    ${pkgs.xorg.libXinerama}/lib:\
    ${pkgs.xorg.libXi}/lib:\
    ${pkgs.xorg.libXxf86vm}/lib:\
    ${pkgs.xorg.libXrender}/lib:\
    ${pkgs.glfw}/lib:$LD_LIBRARY_PATH
  '';
}
