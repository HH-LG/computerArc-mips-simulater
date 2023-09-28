.text
main:   
        # 初始化寄存器
        addiu   $2, $zero, 1024
        addiu   $3, $zero, 512
        addiu   $4, $zero, 256

        # 测试add指令
        add     $5, $2, $3

        # 测试addu指令
        addu    $6, $4, $2

        # 测试sub指令
        sub     $7, $3, $2

        # 测试subu指令
        subu    $8, $2, $4

        # 测试mult指令，结果在HI和LO寄存器中
        mult    $3, $4
        mfhi    $9      # 将乘法结果的高32位取出到$9寄存器
        mflo    $10     # 将乘法结果的低32位取出到$10寄存器

        # 测试multu指令，结果在HI和LO寄存器中
        multu   $4, $2
        mfhi    $11     # 将乘法结果的高32位取出到$11寄存器
        mflo    $12     # 将乘法结果的低32位取出到$12寄存器

        # 测试div指令，商在LO寄存器中，余数在HI寄存器中
        div     $2, $3
        mfhi    $13     # 将除法余数取出到$13寄存器
        mflo    $14     # 将除法商取出到$14寄存器

        # 测试divu指令，商在LO寄存器中，余数在HI寄存器中
        divu    $3, $4
        mfhi    $15     # 将除法余数取出到$15寄存器
        mflo    $16     # 将除法商取出到$16寄存器

        # 系统调用退出程序
        addiu   $v0, $zero, 0xa
        syscall
