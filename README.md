# BPlusTreeConsole

<h3>B+ Tree</h3>

<p>
b+樹為平衡多叉排序樹,只在葉子節點儲存數據,非葉子節點儲存key值,key值順序放在節點上。
<br><br>
每一個節點為磁碟中的一個頁,這樣可以增加每一頁能儲存key的數量,降低查詢時I/O次數同時降低整棵樹的高度。
<br><br>
葉子節點之間都有指針可以方便範圍搜尋。

<p>
<div>References : </div>
<ul>
<li>https://www.geeksforgeeks.org/introduction-of-b-tree/</li>
<li>https://blog.csdn.net/yin767833376/article/details/81511377</li>
<li>https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html</li>
</ul>
</p>
