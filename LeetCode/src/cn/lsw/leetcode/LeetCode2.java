package cn.lsw.leetcode;
/*
 * 2.两数相加
 * 给出两个非空的链表用来表示两个非负的整数。其中，它们各自的位数是按照逆序的方式存储的，
 * 并且它们的每个节点只能存储一位数字。
 * 如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。
 * 您可以假设除了数字 0 之外，这两个数都不会以 0 开头。
示例：
输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 0 -> 8
原因：342 + 465 = 807
来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/add-two-numbers
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
 */

import java.util.LinkedList;
import java.util.List;

public class LeetCode2 {
    public static void main(String[] args) {
        DoubleLinkedList dl = new DoubleLinkedList();
        dl.init();
        dl.add(2, 4);
        dl.add(4, 4);
        dl.add(3, 4);
        dl.insert(1,5,4);
        dl.insert(1,6,4);
        dl.insert(4,6,4);

        dl.remove(5, 4);
        dl.printDoubleLinkedList();

    }
}

/**
 * 双向链表节点
 * @param <T>
 */
class Node<T> {
    Node prev;
    Node next;
    T data;

    public Node() {

    }

    public Node(T data) {
        this.data = data;
    }
}

/**
 * 双向链表
 * @param <I>
 */
class DoubleLinkedList<I> {
    private Node first;
    private Node last;
    private Node current;
    private Integer size;
    private int count;

    /**
     * 链表初始化
     */
    public void init() {
        this.first = null;
        this.last = null;
        this.size = 0;
        this.count = 0;
    }

    /**
     * 向链表末尾添加元素
     * @param data
     * @param size
     */
    public void add(I data, Integer size) {
        Node newNode = new Node(data);
        newNode.next = null;
        newNode.prev = this.last;
        if(this.first == null) {
            this.first = newNode;
        } else {
            this.last.next = newNode;
        }
        this.last = newNode; //把链表尾结点指向给新节点
        ++(this.count);
        this.size += size;
    }

    /**
     * 删除节点，返回true成功，false失败
     * @param index
     * @param size
     * @return
     */
    public boolean remove(int index, Integer size) {
        if(this.size == 0 || index >= this.count || index < 0 ) {
            return false;
        } else if(this.last == this.first) {  //链表只有一个节点
            this.first = null;
            this.last = null;
            this.size = 0;
            this.count = 0;
            return true;
        } else if(index == 0) {   //删除头节点
            this.first = this.first.next;
            --(this.count);
            this.size -= size;
            return true;
        } else if(index == count-1) { //删除尾结点
            this.last = this.last.prev;
            --(this.count);
            this.size -= size;
            return true;
        } else {      //正常情况删除
            this.current = this.first;
            for (int i = 0; i < index ; i++) {
                current = current.next;
            }
            this.current.prev.next = this.current.next;
            this.current.next.prev = this.current.prev;
            --(this.count);
            this.size -= size;
            return true;
        }
    }

    /**
     * 向链表中添加元素，成功返回true，失败返回false
     * @param index
     * @param data
     * @param size
     * @return
     */
    public boolean insert(int index, I data, Integer size) {
        Node newNode = new Node(data);
        if(index < 0) {  //无效索引值
            return false;
        } else if(this.size == 0) {  //链表为空
            this.first = this.last = newNode;
            ++(this.count);
            this.size += size;
            return true;
        } else if(index > count-1) {  //索引值越界
            return false;
        } else {  //正常插入
            this.current = this.first;
            for (int i = 0; i < index ; i++) {
                current = current.next;
            }
            newNode.prev = this.current.prev;
            newNode.next = this.current.prev.next;
            this.current.prev.next = newNode;
            this.current.prev = newNode;
            this.current = newNode;
            ++(this.count);
            this.size += size;
            return true;
        }
    }

    /**
     * 打印链表
     */
    public void printDoubleLinkedList() {
        this.current = this.first;
        while (this.current != null) {
            System.out.print(this.current.data);
            this.current = current.next;
        }
    }
}