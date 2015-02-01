<?php
namespace aquagear\file;

/**
 * ファイル操作のためのクラス.
 * 
 * @package aquagear
 * @subpackage file
 * 
 * @author motonari
 * @version $Id: FileOperator.inc 534 2011-07-26 13:48:33Z motonari $
 */
class FileOperator extends CoreFile {	
	/**
	 * ファイルの削除.
	 * 
	 * @param string $path　ファイルパス
	 * @param bool $error PHPエラー出力の有無
	 * @return bool
	 */
	public static function rm($path, $error=true) {
		$ret = false;
		
		if(is_file($path)) {
			if($error) {
				$ret = unlink($path);
			}
			else {
				$ret = @unlink($path);
			}
		}
		
		return $ret;
	}
	
	/**
	 * ファイル移動
	 * 
	 * @param string $from 移動元ファイルパス
	 * @param string $to 移動先ファイルパス
	 * @param bool $error PHPエラー出力の有無
	 * @return bool
	 */
	public static function mv($from, $to, $error=true) {
		$ret = false;
		
		if(is_file($from)) {
			if($error){ 
				$ret = rename($from, $to);
			}
			else {
				$ret = @rename($from, $to);
			}
		}
		
		return $ret;
	}

	/**
	 * ファイルのリストを取得.
	 * 
	 * @param string $path ディレクトリパス
	 * @param bool $error PHPエラー出力の有無
	 * @return array
	 */
	public static function ls($path="", $error=true) {
		$ret = false;
		
		if(is_dir($path)) {
			if($error) {
				$ret = scandir($path);
			}
			else {
				$ret = @scandir($path);
			}
		}
		
		return $ret;
	}
	
	/**
	 * ファイルのコピー.
	 * 
	 * @param string $from 移動元ファイルパス
	 * @param string $to 移動先ファイルパス
	 * @param bool $error PHPエラー出力の有無
	 * @return bool
	 */
	public static function cp($from, $to, $error=true) {
		$ret = false;
		if(is_file($from)) {
			if($error) {
				$ret = copy($from, $to);
			}
			else {
				$ret = @copy($from, $to);
			}
		}
		
		return $ret;
	}
	
	/**
	 * ファイルのタイムの更新.
	 * 
	 * @param string $path ファイルパス.
	 * @param bool $error PHPエラー出力の有無
	 * @return bool
	 */
	public static function touch($path, $error=true) {
		$ret = false;
		
		if($error) {
			$ret = touch($path);
		}
		else {
			$ret = @touch($path);
		}
		
		return $ret;
	}
	
	/**
	 * ディレクトリの作成.
	 * 
	 * @param string $path ディレクトリパス.
	 * @param int $mode モード
	 * @param bool $recursive 再起処理をするかどうか.
	 * @param bool $error PHPエラー出力の有無
	 * @return bool
	 */
	public static function mkdir($path, $mode=0700, $recursive=false, $error=true) {
		$ret = false;
		
		if($error) {
			$ret = mkdir($path, $mode, $recursive);
		}
		else {
			$ret = @mkdir($path, $mode, $recursive);
		}
		
		return $ret;
	}
	
	/**
	 * atimeの取得.
	 * 
	 * @param string $path ファイルパス.
	 * @return int
	 */
	public static function getAtime($path) {
		if(is_file($path)) {
			return fileatime($path);
		}
		
		return false;
	}
	
	/**
	 * ctimeの取得.
	 * 
	 * @param string $path ファイルパス.
	 * @return int
	 */
	public static function getCtime($path) {
		if(is_file($path)) {
			return filectime($path);
		}
		
		return false;
	}
	
	/**
	 * mtimeの取得.
	 * 
	 * @param string $path ファイルパス.
	 * @return int
	 */
	public static function getMtime($path) {
		if(is_file($path)) {
			return filemtime($path);
		}
		
		return false;
	}
}
?>