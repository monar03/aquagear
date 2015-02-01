<?php 
namespace aquagear\log;

/**
 * ログクラスの基底クラス.
 * 
 * @package aquagear
 * @subpackage log
 * 
 * @author motonari
 * @version $Id: ILogger.inc 534 2011-07-26 13:48:33Z motonari $
 * 
 */
interface ILogger {
	/**
	 * Loggerクラスの初期化.
	 * 
	 * @param mixed $propery 設定値
	 * @param object $conf_obj 設定ファイルオブジェクト
	 * @return void
	 */
	public function initLogger($conf_arr);
	
	/**
	 * デバックレベルの出力.
	 * 
	 * @param string $log_str ログ出力
	 * @return void
	 */
	public function debug($log_str);
	
	/**
	 * infoレベルの出力.
	 * 
	 * @param string $log_str ログ出力
	 * @return void
	 */
	public function info($log_str);
	
	/**
	 * warningレベルの出力.
	 * 
	 * @param string $log_str ログ出力
	 * @return void
	 */
	public function warning($log_str);
	
	/**
	 * errorレベルの出力.
	 * 
	 * @param string $log_str ログ出力
	 * @return void
	 */
	public function error($log_str);
	
	/**
	 * fatalレベルの出力.
	 * 
	 * @param string $log_str ログ出力
	 * @return void
	 */
	public function fatal($log_str);
}
?>
