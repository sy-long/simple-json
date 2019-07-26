#  一个可靠\符合标准\轻量级的 JSON 库 #

**注意：本项目为“学习向”的产品！**
**注意：本项目为“学习向”的产品！**
**注意：本项目为“学习向”的产品！**


其特性如下：

* 符合标准的 JSON 解析器和生成器
* 手写的递归下降解析器（recursive descent parser）
* 使用标准 C++ 语言（C++ 11）
* 仅支持以 `double` 存储 JSON number 类型
* 解析器和生成器的代码合共少于 500 行
* 采用测试驱动开发（test driven development, TDD）模式
* 严格按照JSON标准编写的测试集，共计192条测试用例

补充说明：
* 测试集（test.cpp）
* 解析器/生成器类声明（simple_json.h）
* 解析器实现文件（simple_json.cpp）
* 生成器实现文件 （simple_json_stringify.cpp）

应用说明：
* 包含文件后你需要创建一个simple_json类对象，调用sim_parse_value("JSON")接口方法去传入你需要解析的JSON数据
* 调用你上一步创建的simple_json类对象的相应方法去获取解析得到的值