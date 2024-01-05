# MinecraftForgeUtils
Easier way to find what does field and method mean in obfuscated minecraft mod code.
Latest builds [seen here](https://github.com/GamebP/MinecraftForgeUtils/releases/).

# Pictures
![image](https://github.com/GamebP/MinecraftForgeUtils/assets/88152063/b1b49837-6605-4fad-a3d6-f8bd831e67c8)
![image](https://github.com/GamebP/MinecraftForgeUtils/assets/88152063/6f30f924-736d-4a9a-8d57-259130669991)
![image](https://github.com/GamebP/MinecraftForgeUtils/assets/88152063/dd8087af-a28e-4b38-97fe-8309981d093b)
from java obfucated class to real answers
```java
protected void func_110147_ax() {
        super.func_110147_ax();
        this.func_110148_a(SharedMonsterAttributes.field_111263_d).func_111128_a(0.30000001192092896);
        this.func_110148_a(SharedMonsterAttributes.field_111267_a).func_111128_a(10.0);
    }
```
to
```java
protected void applyEntityAttributes() {
        super.applyEntityAttributes();
        this.getEntityAttribute(SharedMonsterAttributes.movementSpeed).setBaseValue(0.30000001192092896);
        this.getEntityAttribute(SharedMonsterAttributes.maxHealth).setBaseValue(10.0);
    }
```

# Building 
- Debug
- x64
