pver
====
## 一個以C實做，類似portversion -IvL=的小工具

雖然說portversion這個工具還蠻方便的， 但是使用ruby這一點，需要額外安裝ruby。 (一般FreeBSD server可能平常不會裝那個)

portversion對比於pkg_version的優勢是 使用BerkeleyDB建立INDEX的db檔案， 但是實際上對比pkg_version使用INDEX檔(純文字)的速度： (以下皆同一台機器執行第二次以後的速度)

```
portversion -vL=
3.064u 1.485s 0:04.33 104.8%    187+6234k 0+6io 0pf+0w
pkg_version -IvL=
0.210u 0.090s 0:00.30 100.0%    42+2224k 0+0io 0pf+0w
```

如果我們能利用C去實做利用portversion的版本比較， 理論上可以用portversion用INDEX產生的database來加速顯示不同版本。 
