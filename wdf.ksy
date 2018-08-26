meta:
  id: wdf
  file-extension: wdf
  endian: le

seq:
  - id: header
    type: wdatafile_header

instances:
  headerindexs:
    pos: header.offset
    type: wdatafile_index
    repeat: expr
    repeat-expr: header.number
    
    
types:
  wdatafile_header:
    seq:
      - id: id
        size: 4
      - id: number
        type: u4
      - id: offset
        type: u4
  wdatafile_index:
    seq:
      - id: uid
        type: u4
      - id: offset
        type: u4
      - id: size
        type: u4
      - id: space
        type: u4
    instances:
      was:
        pos: offset
        type: wasfile
  wasfile:
    seq:
      - id: header
        type: wasfile_header
  wasfile_header:
    seq:
      - id: magic
        size: 2
      - id: headersize
        type: u2
      - id: img_dir_cnt
        type: u2
      - id: img_frame_cnt
        type: u2
      - id: img_width
        type: u2
      - id: img_height
        type: u2
      - id: img_kp_x
        type: u2
      - id: img_kp_y
        type: u2
      - id: palette
        size: 512
      - id: img_index
        size: img_dir_cnt * img_frame_cnt * 4
      - id: imgs
        type: image
        repeat: expr
        repeat-expr: img_dir_cnt * img_frame_cnt
  image:
    seq:
      - id: kp_x
        type: u4
      - id: kp_y
        type: u4
      - id: width
        type: u4
      - id: height
        type: u4
      - id: line_offsets
        type: u4
        repeat: expr
        repeat-expr: height
      
        
    
        
        
      
        