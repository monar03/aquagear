<?php 
namespace aquagear\file;

/**
 * ファイルリーダーライタークラス.
 * 
 * @package aquagear
 * @subpackage file
 * 
 * @author motonari
 * @version $Id: FileReaderWriter.inc 534 2011-07-26 13:48:33Z motonari $
 */
class FileReaderWriter extends CoreFile {
	/**
	 * ファイルの書き込み.
	 * 
	 * @param string $filename ファイルパス
	 * @param string $value 書き込みの値
	 * @param int $flag writeオプション
	 * @param bool $error PHPエラー出力の有無
	 * @return int
	 */
	public static function write($path, $value, $flag=LOCK_EX, $error=true) {
		$ret = 0;
		
		if($error) {
			$ret = file_put_contents($path, $value, $flag);
		}
		else {
			$ret = @file_put_contents($path, $value, $flag);
		}
		
		if($ret !== false) {
			$ret = true;
		}
		
		return $ret;
	}
	
	/**
	 * ファイルの内容を取得する.
	 * 
	 * @param string $path ファイルパス.
	 * @param int $flag ファイルの取得オプション.
	 * @param bool $error PHPエラー出力の有無
	 * @return string
	 */
	public static function readByString($path, $flag=0, $error=true) {
		$ret = false;
		
		if(is_file($path)) {
			if($error) {
				$ret = file_get_contents($path, $flag);
			}
			else {
				$ret = @file_get_contents($path, $flag);
			}
		}
		
		return $ret;
	}
	
	/**
	 * ファイルの内容を配列に入れて取得する.
	 * 
	 * @param string $path ファイルパス.
	 * @param int $flag ファイルの取得オプション.
	 * @param bool $error PHPエラー出力の有無
	 * @return array
	 */
	public static function readByArray($path, $flag=0, $error=false) {
		$ret = false;
		
		if(is_file($path)) {
			if($error) {
				$ret = file($path, $flag);
			}
			else {
				$ret = @file($path, $flag);
			}
		}
		
		return $ret;
	}
	
	/**
	 * ファイルの内容を標準出力で表示する.
	 * 
	 * @param string $path ファイルパス.
	 * @param bool $flag include_pathを捜索する.
	 * @return bool
	 */
	public static function readByStdout($path, $flag=false, $error=true) {
		$ret = false;

		if(is_file($path)) {
			if($error) {
				$ret = readfile($path, $flag);
			}
			else {
				$ret = @readfile($path, $flag);
			}
		}
		
		return $ret;
	}
}
?>