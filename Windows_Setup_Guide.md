# 《契约象限》UE5 C++ 工程 Windows 平台运行指南

如果您将本项目克隆（Clone）或复制到了一台 **Windows 电脑**上，因为操作系统的变化，C++ 编译环境也会大不相同。请严格按照以下步骤配置您的环境。

---

## 阶段一：安装环境与工具

在 Windows 上运行 UE5 的 C++ 工程，必须安装微软的 **Visual Studio 2022**，而不是 Mac 上的 Xcode。

### 1. 安装 Visual Studio 2022（C++ 编译器）
1. 访问微软官网，下载 [Visual Studio 2022 社区版 (Community)](https://visualstudio.microsoft.com/zh-hans/vs/)（免费）。
2. 运行下载的 `VisualStudioSetup.exe`。
3. 当进入**“工作负荷 (Workloads)”**选择界面时，向下滚动找到“游戏”分类，**务必勾选【使用 C++ 的游戏开发 (Game development with C++)】**。
4. 查看界面**右侧**的“安装详细信息”面板，确保以下组件处于勾选状态：
   * C++ 核心功能
   * **Windows 10 SDK** 或 **Windows 11 SDK**
   * 用于虚幻引擎的 C++ 探查工具
5. 点击右下角的“安装”，这可能需要一段时间。安装完成后，系统可能会提示重启电脑。

### 2. 安装 Epic Games 启动器 与 Unreal Engine 5
1. 前往 Epic Games 官网下载 [Epic Games Launcher](https://store.epicgames.com/) 并安装。
2. 打开启动器，登录您的账号（或注册新账号）。
3. 点击左侧边栏的 **“虚幻引擎”**，然后点击顶部的 **“库”**。
4. 点击加号 `+`，安装 **Unreal Engine 5.3**（推荐，因为代码兼容性最好）。
5. 等待庞大的引擎文件下载并安装完毕。

---

## 阶段二：编译与启动游戏

当您把 Gitee/GitHub 上的代码拉取（Pull）到本地 Windows 电脑后，在文件夹里你会看到 `ContractQuadrant.uproject`。

### 1. 生成 Visual Studio 工程文件
1. 找到游戏目录里的蓝色工程文件：`ContractQuadrant.uproject`。
2. **右键点击** 它。
3. 在弹出的菜单中选择 **“Generate Visual Studio project files”**（如果使用的是 Win 11，可能需要先点击“显示更多选项”才能看到）。
4. 系统会弹出一个黑色的命令行窗口闪过，随后在目录里会生成一个后缀名为 `.sln` 的文件（`ContractQuadrant.sln`）。

### 2. 编译 C++ 代码（方法A：编辑器自动编译 - 最简单）
1. 直接双击运行 `ContractQuadrant.uproject`。
2. 引擎会提示你：“The following modules are missing or built with a different engine version... Would you like to rebuild them now?”
3. 点击 **“是 (Yes)”**。
4. 这个过程会在后台调用 Visual Studio 的编译器，通常需要等待 2~5 分钟。进度条走完后，虚幻5的庞大编辑器界面就会自动打开！

### 3. 编译 C++ 代码（方法B：使用 VS2022 手动编译 - 适合排错）
如果你在使用“方法A”时遇到了报错（弹窗说 Cannot compiled），可以通过这种方式查错：
1. 双击刚生成的 `ContractQuadrant.sln`，这会打开 Visual Studio 2022。
2. 在软件顶部的工具栏中，找到“解决方案配置”下拉框，将其设置为 **Development Editor**。
3. 再找到“解决方案平台”下拉框，将其设置为 **Win64**。
4. 在右侧的“解决方案资源管理器”窗口中，右键点击 `ContractQuadrant` 游戏项目，选择 **“生成 (Build)”**。
5. 去喝杯水，耐心等待下方“输出”窗口提示“生成: 成功”。
6. 成功后，按键盘上的 **F5** 键，就能直接拉起虚幻5编辑器并启动游戏！

---

## 阶段三：导入数据与其他工作

在引擎编辑器完美启动后，所有的工作流程就和在 Mac 电脑上一模一样了：

1. **导入配置表**：
   在编辑器底部的 Content Browser（内容浏览器）里新建一个名为 `DataTables` 的文件夹。
   把电脑 `Data` 文件夹下的 `DT_Skills.json` 和 `DT_CrystalBeasts.json` 拖拽进引擎里，导入类型分别选择 `CQBeastSkillData` 和 `CQBeastBaseData`。
2. **关联模型**：
   导入那些您制作精美的 3D 晶兽模型，并在晶兽代码的默认面板里（或者创建 Blueprints 蓝图），把模型网格体（Mesh）挂载上去。
3. **点击 Play（运行）测试游戏！**
