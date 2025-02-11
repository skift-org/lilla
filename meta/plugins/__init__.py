from cutekit import ensure

ensure((0, 8, 0))

from cutekit import  builder, cli, shell, model

@cli.command(None, "boot", "Boot the kernel")
def bootCmd(args : model.RegistryArgs) -> None:
    registry = model.Registry.use(args)

    target = registry.lookup('kernel-riscv32', model.Target)
    assert target is not None

    component = registry.lookup('kiss-kernel', model.Component)
    assert component is not None

    scope = builder.TargetScope(registry, target)
    kernelProduct = builder.build(scope, component)[0]

    qemu = [
        "qemu-system-riscv32",
        "-machine", "virt",
        "-bios", "default",
        "-nographic",
        "-serial", "mon:stdio",
        "--no-reboot",
        "-d", "int",
        "-kernel", str(kernelProduct.path)
    ]

    shell.exec(*qemu)
