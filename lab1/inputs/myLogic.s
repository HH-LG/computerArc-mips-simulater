.text
main:   
        # 初始化寄存器
        addiu   $2, $zero, 1024
        addiu   $3, $zero, 512
        addiu   $4, $zero, 256

        # 测试sll指令
        sll     $5, $2, 2

        # 测试srl指令
        srl     $6, $3, 2

        # 测试sra指令
        sra     $7, $4, 2

        # 测试sllv指令
        sllv    $8, $2, $3

        # 测试srlv指令
        srlv    $9, $3, $4

        # 测试srav指令
        srav    $10, $4, $2

        # 测试and指令
        and     $11, $2, $3

        # 测试or指令
        or      $12, $3, $4

        # 测试xor指令
        xor     $13, $4, $2

        # 测试nor指令
        nor     $14, $2, $3

        # 系统调用退出程序
        addiu   $v0, $zero, 0xa
        syscall
