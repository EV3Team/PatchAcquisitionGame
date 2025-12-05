import matplotlib.pyplot as plt

# 설정: 캔버스 준비
fig, ax = plt.subplots(figsize=(10, 10)) # 고화질 출력을 위한 크기 설정
ax.set_facecolor('white')

# 4x4 박스를 만들기 위한 5개의 라인 좌표 (0부터 4까지)
grid_range = range(5)

# 디자인 설정
tape_color = '#FFD700' # 노란색 테이프 색상 (Gold/Yellow)
line_width = 25        # 선 굵기 (출력시 잘 보이도록 조정)

# 1. 수직선 그리기
for x in grid_range:
    ax.vlines(x, 0, 4, colors=tape_color, linewidth=line_width, zorder=1)

# 2. 수평선 그리기
for y in grid_range:
    ax.hlines(y, 0, 4, colors=tape_color, linewidth=line_width, zorder=1)

# 3. 마커(색종이) 데이터 정의
# (col, row)는 0부터 시작, 위에서부터 row 1, 2, 3...
markers = [
    # 첫 번째 가로줄 (위에서 1번째 교차선)
    {'col': 1, 'row': 1, 'color': 'green'},
    {'col': 2, 'row': 1, 'color': 'red'},
    {'col': 3, 'row': 1, 'color': 'green'},
    
    # 두 번째 가로줄
    {'col': 1, 'row': 2, 'color': 'red'},
    {'col': 2, 'row': 2, 'color': 'green'},
    {'col': 3, 'row': 2, 'color': 'red'},
    
    # 세 번째 가로줄
    {'col': 1, 'row': 3, 'color': 'green'},
    {'col': 2, 'row': 3, 'color': 'red'},
    {'col': 3, 'row': 3, 'color': 'red'},
    
    # 네 번째 가로줄 (맨 아래)
    {'col': 2, 'row': 4, 'color': 'green'},
]

# 4. 마커 그리기
marker_size = 1200 # 마커 크기
for m in markers:
    # 좌표계 변환: y축은 아래에서 위로 증가하므로, 위쪽을 기준(row)으로 하기 위해 (4 - row) 계산
    ax.scatter(m['col'], 4 - m['row'], s=marker_size, c=m['color'], marker='s', zorder=2, edgecolors='none')

# 5. 마무리 (여백 및 축 제거)
ax.set_xlim(-0.1, 4.1)
ax.set_ylim(-0.1, 4.1)
ax.axis('off') # 눈금, 테두리 제거
plt.gca().set_aspect('equal', adjustable='box')

# 이미지 저장
plt.savefig('printable_grid_clean.png', bbox_inches='tight', pad_inches=0.1, dpi=300)
plt.show()