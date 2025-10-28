#!/usr/bin/env python3
"""
扩展版表达式求值器（Lab2 选作内容）

特性：
1. 支持运算符：+、-、*、/、^（乘方）、一元负号、=（右结合赋值）
2. 操作数可以是变量（以字母或下划线开头）
3. 操作数支持浮点数
4. 提供 Tkinter 图形界面，展示求值结果、后缀表达式、变量表与栈操作过程

实现思路：
 - 词法分析：将输入拆分为数字、变量、括号、运算符
 - Shunting-Yard 算法：利用运算符栈生成后缀表达式
 - 后缀表达式求值：使用栈进行计算，同时记录每一步栈状态
"""

from __future__ import annotations

import math
import tkinter as tk
from tkinter import ttk, messagebox
from typing import Dict, Iterable, List, Sequence, Tuple


Token = Tuple[str, object]


class ExpressionEvaluator:
    """基于栈的表达式求值器。"""

    def __init__(self) -> None:
        # 变量表
        self.variables: Dict[str, float] = {}
        # 运算符优先级（数值越大优先级越高）
        self.precedence = {
            "=": 1,
            "+": 2,
            "-": 2,
            "*": 3,
            "/": 3,
            "^": 5,
            "u-": 4,  # 一元负号
        }
        self.associativity = {
            "=": "right",
            "+": "left",
            "-": "left",
            "*": "left",
            "/": "left",
            "^": "right",
            "u-": "right",
        }

    # ----------------- 词法分析 -----------------
    def tokenize(self, expression: str) -> List[Token]:
        expr = expression.strip()
        # 若用户习惯以 "=" 结束表达式，可兼容处理（仅保留末尾一个 =）
        if expr.endswith("=") and expr.count("=") == 1:
            expr = expr[:-1].rstrip()

        tokens: List[Token] = []
        i = 0
        prev_type = "start"  # start, value, operator, lparen

        while i < len(expr):
            ch = expr[i]

            if ch.isspace():
                i += 1
                continue

            # 数字（浮点数）
            if ch.isdigit() or ch == ".":
                start = i
                dot_count = 0
                while i < len(expr) and (expr[i].isdigit() or expr[i] == "."):
                    if expr[i] == ".":
                        dot_count += 1
                        if dot_count > 1:
                            raise ValueError(f"无效的数字常量：{expr[start:i+1]}")
                    i += 1
                number_text = expr[start:i]
                if number_text in {".", ""}:
                    raise ValueError("数字格式错误")
                tokens.append(("number", float(number_text)))
                prev_type = "value"
                continue

            # 变量名（字母或下划线开头，后续可带数字）
            if ch.isalpha() or ch == "_":
                start = i
                i += 1
                while i < len(expr) and (expr[i].isalnum() or expr[i] == "_"):
                    i += 1
                name = expr[start:i]
                tokens.append(("var", name))
                prev_type = "value"
                continue

            # 左右括号
            if ch == "(":
                tokens.append(("lparen", ch))
                prev_type = "lparen"
                i += 1
                continue

            if ch == ")":
                tokens.append(("rparen", ch))
                prev_type = "value"
                i += 1
                continue

            # 运算符
            if ch in "+-*/^=":
                if ch == "-":
                    if prev_type in {"start", "operator", "lparen"}:
                        tokens.append(("op", "u-"))
                    else:
                        tokens.append(("op", "-"))
                else:
                    tokens.append(("op", ch))
                prev_type = "operator"
                i += 1
                continue

            raise ValueError(f"无法识别的字符：{ch}")

        return tokens

    # ----------------- 中缀转后缀 -----------------
    def to_postfix(self, tokens: Sequence[Token]) -> List[Token]:
        output: List[Token] = []
        op_stack: List[Token] = []

        for token in tokens:
            kind, value = token

            if kind in {"number", "var"}:
                output.append(token)
            elif kind == "op":
                while op_stack:
                    top_kind, top_value = op_stack[-1]
                    if top_kind != "op":
                        break
                    if self._should_pop(top_value, value):
                        output.append(op_stack.pop())
                    else:
                        break
                op_stack.append(token)
            elif kind == "lparen":
                op_stack.append(token)
            elif kind == "rparen":
                while op_stack and op_stack[-1][0] != "lparen":
                    output.append(op_stack.pop())
                if not op_stack:
                    raise ValueError("括号不匹配：缺少 '('")
                op_stack.pop()  # 丢弃左括号
            else:
                raise ValueError("未知的记号类型")

        while op_stack:
            kind, value = op_stack.pop()
            if kind in {"lparen", "rparen"}:
                raise ValueError("括号不匹配：缺少 ')'")
            output.append((kind, value))

        return output

    # ----------------- 后缀求值 -----------------
    def evaluate(self, expression: str) -> Tuple[float, List[str], List[str]]:
        """
        计算表达式，返回 (结果, 栈操作日志, 后缀表达式显示).
        """
        tokens = self.tokenize(expression)
        if not tokens:
            raise ValueError("请输入表达式")

        postfix = self.to_postfix(tokens)
        result, logs = self._compute_postfix(postfix)
        postfix_display = [self._format_token(tok) for tok in postfix]
        return result, logs, postfix_display

    # ----------------- 工具方法 -----------------
    def _should_pop(self, top_op: str, current_op: str) -> bool:
        top_prec = self.precedence[top_op]
        curr_prec = self.precedence[current_op]
        if self.associativity[current_op] == "left":
            return top_prec >= curr_prec
        return top_prec > curr_prec

    def _compute_postfix(self, postfix: Iterable[Token]) -> Tuple[float, List[str]]:
        stack: List[Token] = []
        logs: List[str] = []

        for token in postfix:
            kind, value = token

            if kind in {"number", "var"}:
                stack.append(token)
                logs.append(self._format_step(f"读入 {self._format_token(token)}", stack))
                continue

            if kind != "op":
                raise ValueError("后缀表达式包含未知记号")

            if value == "u-":
                if not stack:
                    raise ValueError("一元运算缺少操作数")
                operand_token = stack.pop()
                operand = self._resolve_value(operand_token)
                result = -operand
                stack.append(("number", result))
                logs.append(self._format_step("执行一元负号", stack))
                continue

            if value == "=":
                if len(stack) < 2:
                    raise ValueError("赋值运算缺少操作数")
                right = stack.pop()
                left = stack.pop()
                if left[0] != "var":
                    raise ValueError("赋值左侧必须是变量名")
                resolved = self._resolve_value(right)
                self.variables[left[1]] = resolved
                stack.append(("number", resolved))
                logs.append(self._format_step(f"赋值 {left[1]} = {resolved}", stack))
                continue

            # 双目运算
            if len(stack) < 2:
                raise ValueError(f"{value} 运算缺少操作数")
            right = self._resolve_value(stack.pop())
            left = self._resolve_value(stack.pop())

            result = self._apply_binary(left, right, value)
            stack.append(("number", result))
            logs.append(self._format_step(f"执行 {value}", stack))

        if len(stack) != 1:
            raise ValueError("表达式不完整，无法得到唯一结果")

        return self._resolve_value(stack[0]), logs

    def _resolve_value(self, token: Token) -> float:
        kind, value = token
        if kind == "number":
            return float(value)
        if kind == "var":
            if value not in self.variables:
                raise ValueError(f"变量 {value} 尚未赋值")
            return self.variables[value]
        raise ValueError("未知的栈元素类型")

    def _apply_binary(self, left: float, right: float, op: str) -> float:
        if op == "+":
            return left + right
        if op == "-":
            return left - right
        if op == "*":
            return left * right
        if op == "/":
            if right == 0:
                raise ValueError("除零错误")
            return left / right
        if op == "^":
            try:
                return math.pow(left, right)
            except ValueError as exc:
                raise ValueError(f"乘方运算非法：{exc}") from exc
        raise ValueError(f"未知的运算符：{op}")

    def _format_token(self, token: Token) -> str:
        kind, value = token
        if kind == "number":
            return f"{float(value):g}"
        if kind == "var":
            return str(value)
        if kind == "op":
            return "-" if value == "u-" else str(value)
        if kind in {"lparen", "rparen"}:
            return str(value)
        return str(value)

    def _format_step(self, action: str, stack: Sequence[Token]) -> str:
        formatted_stack = ", ".join(self._format_token(tok) for tok in stack)
        return f"{action} -> 栈: [{formatted_stack}]"


class CalculatorGUI:
    """基于 Tkinter 的图形界面。"""

    def __init__(self, root: tk.Tk) -> None:
        self.root = root
        root.title("表达式求值器 (Lab2 扩展)")

        self.evaluator = ExpressionEvaluator()

        self.expr_var = tk.StringVar()
        self.result_var = tk.StringVar(value="结果将显示在这里")

        self._build_widgets()

    def _build_widgets(self) -> None:
        main_frame = ttk.Frame(self.root, padding="16 16 16 16")
        main_frame.grid(row=0, column=0, sticky="nsew")

        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        # 输入区域
        ttk.Label(main_frame, text="请输入表达式：").grid(row=0, column=0, sticky="w")
        entry = ttk.Entry(main_frame, textvariable=self.expr_var, width=40)
        entry.grid(row=1, column=0, columnspan=2, sticky="ew", pady=(4, 8))
        entry.bind("<Return>", self._on_evaluate)

        ttk.Button(main_frame, text="计算", command=self._on_evaluate).grid(
            row=1, column=2, padx=(8, 0)
        )

        # 结果显示
        ttk.Label(main_frame, text="计算结果：").grid(row=2, column=0, sticky="w")
        result_label = ttk.Label(main_frame, textvariable=self.result_var, foreground="#1a73e8")
        result_label.grid(row=3, column=0, columnspan=3, sticky="w", pady=(0, 12))

        # 后缀表达式
        ttk.Label(main_frame, text="后缀表达式：").grid(row=4, column=0, sticky="w")
        self.postfix_var = tk.StringVar(value="")
        postfix_label = ttk.Label(main_frame, textvariable=self.postfix_var)
        postfix_label.grid(row=5, column=0, columnspan=3, sticky="w")

        # 栈操作日志
        ttk.Label(main_frame, text="栈操作过程：").grid(row=6, column=0, sticky="w", pady=(12, 0))
        self.log_text = tk.Text(main_frame, height=12, width=60, wrap="word")
        self.log_text.grid(row=7, column=0, columnspan=3, sticky="nsew")
        self.log_text.configure(state="disabled")

        # 变量表
        ttk.Label(main_frame, text="当前变量表：").grid(row=8, column=0, sticky="w", pady=(12, 0))
        self.var_tree = ttk.Treeview(main_frame, columns=("value",), show="headings", height=5)
        self.var_tree.heading("value", text="值")
        self.var_tree.grid(row=9, column=0, columnspan=3, sticky="nsew")

        # 历史记录
        ttk.Label(main_frame, text="历史记录：").grid(row=10, column=0, sticky="w", pady=(12, 0))
        self.history_list = tk.Listbox(main_frame, height=6)
        self.history_list.grid(row=11, column=0, columnspan=3, sticky="nsew")

        # 布局权重
        main_frame.columnconfigure(0, weight=3)
        main_frame.columnconfigure(1, weight=1)
        main_frame.columnconfigure(2, weight=0)
        main_frame.rowconfigure(7, weight=1)
        main_frame.rowconfigure(9, weight=1)
        main_frame.rowconfigure(11, weight=1)

    def _on_evaluate(self, event=None) -> None:
        expr = self.expr_var.get().strip()
        if not expr:
            messagebox.showinfo("提示", "请输入表达式")
            return

        try:
            result, logs, postfix_display = self.evaluator.evaluate(expr)
        except ValueError as exc:
            messagebox.showerror("错误", str(exc))
            return

        formatted_result = f"{result:g}"
        self.result_var.set(formatted_result)
        self.postfix_var.set(" ".join(postfix_display))
        self._update_log(logs)
        self._update_variables()
        self._append_history(expr, formatted_result)

    def _update_log(self, logs: Sequence[str]) -> None:
        self.log_text.configure(state="normal")
        self.log_text.delete("1.0", tk.END)
        for line in logs:
            self.log_text.insert(tk.END, line + "\n")
        self.log_text.configure(state="disabled")

    def _update_variables(self) -> None:
        for item in self.var_tree.get_children():
            self.var_tree.delete(item)
        for name, value in sorted(self.evaluator.variables.items()):
            self.var_tree.insert("", tk.END, values=(f"{name} = {value:g}",))

    def _append_history(self, expr: str, result: str) -> None:
        self.history_list.insert(tk.END, f"{expr} = {result}")
        self.history_list.see(tk.END)


def main() -> None:
    root = tk.Tk()
    CalculatorGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
