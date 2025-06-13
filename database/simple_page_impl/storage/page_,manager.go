package storage

import "os"

type PageManager struct {
	file     *os.File
	pageSize int
}

func NewPageManager(path string) (*PageManager, error) {
	file, err := os.OpenFile(path, os.O_RDWR|os.O_CREATE, 0644)
	if err != nil {
		return nil, err
	}
	return &PageManager{
		file:     file,
		pageSize: PAGE_SIZE,
	}, nil
}

func (pm *PageManager) ReadPage(pageID uint32) (*Page, error) {
	offset := int64(pageID) * int64(pm.pageSize)
	buf := make([]byte, pm.pageSize)

	_, err := pm.file.ReadAt(buf, offset)
	if err != nil {
		return nil, err
	}

	return DeserializePage(buf), nil
}

func (pm *PageManager) WritePage(p *Page) error {
	offset := int64(p.ID) * int64(pm.pageSize)
	buf := p.Serialize()

	_, err := pm.file.WriteAt(buf, offset)
	return err
}

func (pm *PageManager) AllocateNewPage() (*Page, error) {
	stat, err := pm.file.Stat()
	if err != nil {
		return nil, err
	}

	pageID := uint32(stat.Size() / int64(pm.pageSize))
	p := &Page{
		ID:              pageID,
		TupleCount:      0,
		FreeSpaceOffset: PAGE_SIZE,
		Slots:           make([]Slot, 0),
		Data:            make([]byte, PAGE_SIZE),
	}
	err = pm.WritePage(p)
	if err != nil {
		return nil, err
	}
	return p, nil
}

func (pm *PageManager) Close() error {
	if pm.file != nil {
		return pm.file.Close()
	}
	return nil
}

func (pm *PageManager) Stats() (numPages uint32, fileSize int64, err error) {
	stat, err := pm.file.Stat()
	if err != nil {
		return 0, 0, err
	}
	fileSize = stat.Size()
	numPages = uint32(fileSize / int64(pm.pageSize))
	return numPages, fileSize, nil
}
