from cutekit import builder, cli, shell, model


@cli.command("boot", "Boot the kernel")
def bootCmd(args: model.RegistryArgs) -> None:
    registry = model.Registry.use(args)

    target = registry.lookup("kernel-riscv64", model.Target)
    assert target is not None

    component = registry.lookup("kernel.core", model.Component)
    assert component is not None

    scope = builder.TargetScope(registry, target)
    kernelProduct = builder.build(scope, component)[0]

    qemu = [
        "qemu-system-riscv64",
        "-nographic",
        "-m", "128M",
        "--no-reboot",
        "-machine", "virt",
        "-cpu", "rv64,h=true",
        "-d", "cpu_reset,unimp,guest_errors,int",
        "-D", "qemu.log",
        "-bios", "default",
        "-serial", "mon:stdio",
        "-kernel", str(kernelProduct.path),
    ]

    shell.exec(*qemu)
