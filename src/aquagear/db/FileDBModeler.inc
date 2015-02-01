<?php
namespace aquagear\db;

/**
 * File DBのモデルクラス.
 * 
 * @package aquagear
 * @subpackage db
 * 
 * @author motonari
 * @version $Id$
 *
 */
class FileDBModeler extends \aquagear\Object {
	/**
	 * ベースパス
	 * 
	 * @var string
	 */
	private $_base_path = null;
	
	/**
	 * ベース
	 * 
	 * @var string
	 */
	const BASE = '';
	
	/**
	 * コンストラクタ
	 * 
	 * @param string $db ディレクトリパス
	 * 
	 * @return void
	 */
	public function __construct($db) {
		$this->_base_path = self::BASE . '/' . $db . '/';
	}
	
	/**
	 * ファイルの書き込み
	 * 
	 * @param string $path パス
	 * @param string $data データ
	 * 
	 * @return int 書き込みバイト
	 */
	private function _write($path, $data) {
		if(! is_dir(dirname($path))) {
			\aquagear\file\FileOperator::mkdir(dirname($path), 0777, true);
		}
		
		return \aquagear\file\FileReaderWriter::write($path, $data, LOCK_EX);
	}
	
	/**
	 * ファイルの読み込み
	 * 
	 * @param string $path パス
	 * @return string 読み込みデータ
	 */
	private function _read($path) {
		return \aquagear\file\FileReaderWriter::readByString($path);
	}
	
	/**
	 * パスの取得
	 * 
	 * @param string $key キー
	 * @return string パス
	 */
	private function _getPath($key) {
		$path = '';
		
		$dirname  = dirname($key);
		$filename = basename($key);
		if(strlen($filename) >= 2) {
			$path = substr($filename, 0, 2);
			if(strlen($filename) >= 4) { 
				$path .= '/' . substr($filename, 2, 2);
			}
			
			$path = $dirname . '/' . $path . '/' . $filename;
		}
		else {
			$path = $key;
		}
		
		if($path === null || $path === '') {
			return '';
		}
		
		return $this->_base_path . $path;
	}
	
	/**
	 * 取得メソッド
	 * 
	 * @param string $key キー
	 * @return string データ
	 */
	public function get($key) {
		$path = $this->_getPath($key);
		if(is_file($path)) {
			return $this->_read($path);
		}

		return false;
	}
	
	/**
	 * セットメソッド
	 * 
	 * @param string $key キー
	 * @param string $data データ
	 * @return int データの書き込みバイト数
	 */
	public function set($key, $data) {
		$path = $this->_getPath($key);
		if($path === null || $path === '') {
			return false;	
		}
		
		return $this->_write($path, $data);
	}
	
	/**
	 * 削除メソッド
	 * 
	 * @param string $key キー
	 * 
	 * @return bool 削除成功時true,失敗時false
	 */
	public function delete($key) {
		$path = $this->_getPath($key);
		if($path === null || $path === '') {
			return false;
		}
		
		return \aquagear\file\FileOperator::rm($path, true);
	}
	
	/**
	 * 存在確認
	 * 
	 * @param string $key キー
	 * @return bool 存在する場合true、存在しない場合false
	 */
	public function exist($key) {
		if(is_file($this->_getPath($key))) {
			return true;
		}
		
		return false;
	}
}
?>