.text
main:   
        # 初始化寄存器
        addiu   $2, $zero, 1024
        addiu   $3, $zero, 512
        addiu   $4, $zero, 256

        # 测试addi指令
        addi    $5, $2, 100

        # 测试addiu指令
        addiu   $6, $3, 100

        # 测试slti指令
        slti    $7, $4, 100

        # 测试sltiu指令
        sltiu   $8, $2, 100

        # 测试andi指令
        andi    $9, $3, 0xFF

        # 测试ori指令
        ori     $10, $4, 0xFF

        # 测试xori指令
        xori    $11, $2, 0xFF

        # 测试lui指令
        lui     $12, 0x1000

        # 系统调用退出程序
        addiu   $v0, $zero, 0xa
        syscall
